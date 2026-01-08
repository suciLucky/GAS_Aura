// Copyright JCS

#pragma once

#include "CoreMinimal.h"
#include "Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraAbilitySystemLibrary.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure,Category="AuraAbilitySystemLibrary|WidgetController")
	static UOverlayWidgetController*GetOverlayWidgetController(const UObject*WorldContextObject);

	UFUNCTION(BlueprintPure,Category="AuraAbilitySystemLibrary|WidgetController")
	static UAttributeMenuWidgetController*GetaAttributeMenuWidgetController(const UObject*WorldContextObject);

	//使用角色种类和等级初始化角色信息
	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLibrary|CharacterClassDefaults")
	static void InitializeDefaultAttributes(const UObject*WorldContextObject,const ECharacterClass CharacterClass,const float Level,UAbilitySystemComponent*ASC);

	//初始化敌人的技能
	UFUNCTION(BlueprintCallable,Category="AuraAbilitySystemLibrary|CharacterClassDefaults")
	static void GiveStartupAbilities(const UObject*WorldContextObject,UAbilitySystemComponent*ASC);
};
