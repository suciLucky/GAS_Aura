// Copyright JCS

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "AuraEffectActor.generated.h"

//GE应用策略（情境）
UENUM(BlueprintType)
enum class EEffectApplicationPolicy:uint8
{
	ApplyOnOverlap,
	ApplyEndOverlap,
	DoNotApply
};

//GE移除策略（情境）
UENUM(BlueprintType)
enum class EEffectRemovalPolicy:uint8
{
	RemoveEndOverlap,
	DoNotRemove
};

UCLASS()
class AURA_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()

public:
	AAuraEffectActor();

protected:
	virtual void BeginPlay() override;

	//将效果应用到目标
	UFUNCTION(BlueprintCallable)
	void ApplyEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	//重叠时处理GE
	UFUNCTION(BlueprintCallable)
	void GameplayEffectOnOverlap(AActor*TargetActor);

	//结束重叠时处理GE
	UFUNCTION(BlueprintCallable)
	void GameplayEffectEndOverlap(AActor*TargetActor);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Apllied Effects")
	bool bDestroyOnEffectRemoval=false;
	
	//需要应用的效果
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Apllied Effects")
	TSubclassOf<UGameplayEffect> InstantGameplayEffectClass;//即时效果

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Apllied Effects")
	EEffectApplicationPolicy InstantEffectApplicationPolicy=EEffectApplicationPolicy::DoNotApply;//即时效果应用策略

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Apllied Effects")
	TSubclassOf<UGameplayEffect> DurationGameplayEffectClass;//持续时间效果

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Apllied Effects")
	EEffectApplicationPolicy DurationEffectApplicationPolicy =EEffectApplicationPolicy::DoNotApply;//持续时间效果应用策略

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Apllied Effects")
	TSubclassOf<UGameplayEffect> InfiniteGameplayEffectClass;//永久效果

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Apllied Effects")
	EEffectApplicationPolicy InfiniteEffectApplicationPolicy=EEffectApplicationPolicy::DoNotApply;//永久效果应用策略

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Apllied Effects")
	EEffectRemovalPolicy InfiniteEffectRemovalPolicy=EEffectRemovalPolicy::DoNotRemove;//永久效果移除策略

	//需要存储的已激活的GE
	TMap<FActiveGameplayEffectHandle,UAbilitySystemComponent*> ActiveEffectHandles;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Apllied Effects")
	float ActorLevel=1.f;
};
