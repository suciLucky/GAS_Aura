// Copyright JCS


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "AuraAbilityTypes.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"

UOverlayWidgetController* UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{	
	if(APlayerController*PC = UGameplayStatics::GetPlayerController(WorldContextObject,0))
	{
		if(AAuraHUD*AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			AAuraPlayerState*PS=PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent*ASC=PS->GetAbilitySystemComponent();
			UAttributeSet*AS=PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC,PS,ASC,AS);
			return AuraHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystemLibrary::GetaAttributeMenuWidgetController(
	const UObject* WorldContextObject)
{
	if(APlayerController*PC = UGameplayStatics::GetPlayerController(WorldContextObject,0))
	{
		if(AAuraHUD*AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			AAuraPlayerState*PS=PC->GetPlayerState<AAuraPlayerState>();
			UAbilitySystemComponent*ASC=PS->GetAbilitySystemComponent();
			UAttributeSet*AS=PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC,PS,ASC,AS);
			return AuraHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

void UAuraAbilitySystemLibrary::InitializeDefaultAttributes(const UObject*WorldContextObject,const ECharacterClass CharacterClass,
	const float Level,UAbilitySystemComponent*ASC)
{
	//获取角色数据
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if(CharacterClassInfo==nullptr)return;
	//设置GE的源对象
	const AActor*AvatarActor = ASC->GetAvatarActor();
	FGameplayEffectContextHandle ContextHandle= ASC->MakeEffectContext();
	ContextHandle.AddSourceObject(AvatarActor);

	//在资产中通过条件查找初始化数据
	const FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	//给特定角色通过其ASC应用初始化数据里的初始化GE
	ASC->ApplyGameplayEffectSpecToSelf(*ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes,Level,ContextHandle).Data.Get());
	//给角色应用数据资产里的通用初始化GE
	ASC->ApplyGameplayEffectSpecToSelf(*ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes,Level,ContextHandle).Data.Get());
	ASC->ApplyGameplayEffectSpecToSelf(*ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes,Level,ContextHandle).Data.Get());
}

void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC,ECharacterClass CharacterClass)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if(CharacterClassInfo==nullptr)return;
	//赋予通用技能
	for(const auto Ability : CharacterClassInfo->CommonAbilities)
	{
		ASC->GiveAbility(FGameplayAbilitySpec(Ability));
	}
	//赋予特有技能
	const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	for (const auto AbilityClass : DefaultInfo.StartupAbilities)
	{		
		if(ICombatInterface* CombatInterface = Cast<ICombatInterface>(ASC->GetAvatarActor()))
		{			
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass,CombatInterface->GetPlayerLevel());
			ASC->GiveAbility(FGameplayAbilitySpec(AbilitySpec));
		}
	}
}

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	const AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	return AuraGameMode->CharacterClassInfo;
}

bool UAuraAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if(const FAuraGameplayEffectContext* AuraContext =static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	    return AuraContext->IsBlockedHit();
	return false;
}

bool UAuraAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if(const FAuraGameplayEffectContext* AuraContext =static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
		return AuraContext->IsCriticalHit();
	return false;
}

void UAuraAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, const bool bInIsBlockedHit)
{
	if(FAuraGameplayEffectContext* AuraContext =static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
		AuraContext->SetIsBlockedHit(bInIsBlockedHit);
}

void UAuraAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle,const
	bool bInIsCriticalHit)
{
	if(FAuraGameplayEffectContext* AuraContext =static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
		AuraContext->SetIsCriticalHit(bInIsCriticalHit);
}

void UAuraAbilitySystemLibrary::GetLivePlayersInRadius(const UObject* WorldContextObject,
	TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorToIgnore, float Radius,
	const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorToIgnore);

	TArray<FOverlapResult> Overlaps;
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);

		for(auto& OverlapResult:Overlaps)
		{					
			if(OverlapResult.GetActor()->Implements<UCombatInterface>()&&!ICombatInterface::Execute_IsDead(OverlapResult.GetActor()))
			{
				OutOverlappingActors.AddUnique(OverlapResult.GetActor());
			}
		}
	}
}
