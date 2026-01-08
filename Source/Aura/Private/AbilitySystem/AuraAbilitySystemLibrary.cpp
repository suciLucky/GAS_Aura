// Copyright JCS


#include "AbilitySystem/AuraAbilitySystemLibrary.h"

#include "Game/AuraGameModeBase.h"
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
	//获取到GameMode以获取初始化数据资产
	const AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if(AuraGameMode==nullptr) return;

	//设置GE的源对象
	const AActor*AvatarActor = ASC->GetAvatarActor();
	FGameplayEffectContextHandle ContextHandle= ASC->MakeEffectContext();
	ContextHandle.AddSourceObject(AvatarActor);
	
	//在资产中通过条件查找初始化数据
	const FCharacterClassDefaultInfo ClassDefaultInfo = AuraGameMode->CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	//给特定角色通过其ASC应用初始化数据里的初始化GE
	ASC->ApplyGameplayEffectSpecToSelf(*ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes,Level,ContextHandle).Data.Get());
	//给角色应用数据资产里的通用初始化GE
	ASC->ApplyGameplayEffectSpecToSelf(*ASC->MakeOutgoingSpec(AuraGameMode->CharacterClassInfo->SecondaryAttributes,Level,ContextHandle).Data.Get());
	ASC->ApplyGameplayEffectSpecToSelf(*ASC->MakeOutgoingSpec(AuraGameMode->CharacterClassInfo->VitalAttributes,Level,ContextHandle).Data.Get());
}

void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{
	const AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if(AuraGameMode==nullptr) return;

	//赋予技能
	for(const auto Ability : AuraGameMode->CharacterClassInfo->CommonAbilities)
	{
		ASC->GiveAbility(FGameplayAbilitySpec(Ability));
	}
}
