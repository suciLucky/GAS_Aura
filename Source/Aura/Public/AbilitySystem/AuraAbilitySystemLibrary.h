// Copyright JCS

#pragma once

#include "CoreMinimal.h"
#include "Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/WidgetController/SpellMenuWidgetController.h"
#include "AuraAbilitySystemLibrary.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure,Category="AuraAbilitySystemLibrary|WidgetController",meta = (DefaultToSelf = "WorldContextObject"))
	static bool MakeWidgetControllerPrams(const UObject*WorldContextObject,FWidgetControllerParams& OutWCPrams,AAuraHUD* & OutAuraHUD);
	
	UFUNCTION(BlueprintPure,Category="AuraAbilitySystemLibrary|WidgetController",meta = (DefaultToSelf = "WorldContextObject"))
	static UOverlayWidgetController*GetOverlayWidgetController(const UObject*WorldContextObject);

	UFUNCTION(BlueprintPure,Category="AuraAbilitySystemLibrary|WidgetController",meta = (DefaultToSelf = "WorldContextObject"))
	static UAttributeMenuWidgetController*GetAttributeMenuWidgetController(const UObject*WorldContextObject);

	UFUNCTION(BlueprintPure,Category="AuraAbilitySystemLibrary|WidgetController",meta = (DefaultToSelf = "WorldContextObject"))
	static USpellMenuWidgetController*GetSpellMenuWidgetController(const UObject*WorldContextObject);

	//使用角色种类和等级初始化角色信息
	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLibrary|CharacterClassDefaults")
	static void InitializeDefaultAttributes(const UObject*WorldContextObject,const ECharacterClass CharacterClass,const float Level,UAbilitySystemComponent*ASC);

	//初始化敌人的技能
	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLibrary|CharacterClassDefaults")
	static void GiveStartupAbilities(const UObject*WorldContextObject,UAbilitySystemComponent*ASC,ECharacterClass CharacterClass);

	//获取角色信息
	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLibrary|CharacterClassDefaults")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject*WorldContextObject);

	//获取攻击是否被阻挡
	UFUNCTION(BlueprintPure,Category="AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);
	//获取攻击是否暴击
	UFUNCTION(BlueprintPure,Category="AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	//设置攻击是否被阻挡
	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsBlockedHit(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle,const bool bInIsBlockedHit);
	//设置攻击是否暴击
	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLibrary|GameplayEffects")
	static void SetIsCriticalHit(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle,const bool bInIsCriticalHit);

	//查找半径内的存活玩家
	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLibrary|GameplayMechanic")
	static void GetLivePlayersInRadius(const UObject*WorldContextObject,TArray<AActor*>& OutOverlappingActors,const TArray<AActor*>& ActorToIgnore,float Radius,const FVector& SphereOrigin);

	//判断两者是否一个阵营
	UFUNCTION(BlueprintPure,Category="AuraAbilitySystemLibrary|GameplayMechanic")
	static bool IsNotFriend(AActor* FirstActor,AActor* SecondActor);

	//根据角色类型和他的等级获取可获得的经验值
	static int32 GetXPRewardForClassAndLevel(const UObject*WorldContextObject,ECharacterClass CharacterClass,int32 CharacterLevel);
};
