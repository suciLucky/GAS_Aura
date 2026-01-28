// Copyright JCS


#include "Actor/AuraEffectActor.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"


AAuraEffectActor::AAuraEffectActor()
{

	PrimaryActorTick.bCanEverTick = false;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>("RootComponent"));


}


void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();
}

void AAuraEffectActor::ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	//判断作用目标
	if(TargetActor->ActorHasTag(FName("Enemy"))&&!bApplyEffectsToEnemies) return; 
	//获取组件
	UAbilitySystemComponent*TargetASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if(TargetASC==nullptr) return;
	check(GameplayEffectClass);
	//创建GE上下文
	FGameplayEffectContextHandle EffectContextHandle =TargetASC->MakeEffectContext();
	//设置源对象
	EffectContextHandle.AddSourceObject(this);
	//创建GE实例
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass,ActorLevel,EffectContextHandle);
	//应用GE，返回一个已激活的Handle
	FActiveGameplayEffectHandle ActiveEffectHandle=TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	//检查GE是不是Infinite且其移除策略是RemoveEndOverlap
	const bool bIsInfinite=EffectSpecHandle.Data.Get()->Def.Get()->DurationPolicy==EGameplayEffectDurationType::Infinite;
	if(bIsInfinite&&InfiniteEffectRemovalPolicy==EEffectRemovalPolicy::RemoveEndOverlap)
	{
		ActiveEffectHandles.Add(ActiveEffectHandle,TargetASC);//存储该种GE
	}

	if(!bIsInfinite)
	{
		Destroy();
	}
	
}

void AAuraEffectActor::GameplayEffectOnOverlap(AActor* TargetActor)
{
	//判断作用目标
	if(TargetActor->ActorHasTag(FName("Enemy"))&&!bApplyEffectsToEnemies) return; 
	if (InstantEffectApplicationPolicy==EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor,InstantGameplayEffectClass);
	}
	if (DurationEffectApplicationPolicy==EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor,DurationGameplayEffectClass);
	}
	if(InfiniteEffectApplicationPolicy==EEffectApplicationPolicy::ApplyOnOverlap)
	{
		ApplyEffectToTarget(TargetActor,InfiniteGameplayEffectClass);
	}
}

void AAuraEffectActor::GameplayEffectEndOverlap(AActor* TargetActor)
{
	//判断作用目标
	if(TargetActor->ActorHasTag(FName("Enemy"))&&!bApplyEffectsToEnemies) return; 
	if (InstantEffectApplicationPolicy==EEffectApplicationPolicy::ApplyEndOverlap)
	{
		ApplyEffectToTarget(TargetActor,InstantGameplayEffectClass);
	}
	if (DurationEffectApplicationPolicy==EEffectApplicationPolicy::ApplyEndOverlap)
	{
		ApplyEffectToTarget(TargetActor,DurationGameplayEffectClass);
	}
	if(InfiniteEffectApplicationPolicy==EEffectApplicationPolicy::ApplyEndOverlap)
	{
		ApplyEffectToTarget(TargetActor,InfiniteGameplayEffectClass);
	}
	//手动移除InfiniteGE
	if(InfiniteEffectRemovalPolicy==EEffectRemovalPolicy::RemoveEndOverlap)
	{
		//获取ASC
		UAbilitySystemComponent*TargetASC=UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
		if(!IsValid(TargetASC)) return;

		//将需要移除的Handle暂时存储
		TArray<FActiveGameplayEffectHandle> HandleToRemove;
		//对于目标Actor，移除其GE
		for(auto HandlePair:ActiveEffectHandles)
		{
			if(TargetASC==HandlePair.Value)
			{
				TargetASC->RemoveActiveGameplayEffect(HandlePair.Key,1);
				HandleToRemove.Add(HandlePair.Key);//存储Handle
			}
		}
		//在已激活GE的Map里移除刚刚存储的Handle
		for (auto &Handle:HandleToRemove)
		{
			ActiveEffectHandles.FindAndRemoveChecked(Handle);
		}
	}
}



