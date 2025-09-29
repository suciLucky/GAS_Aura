// Copyright JCS

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
/**
 * 
 */
UCLASS()

class AURA_API AAuraPlayerState : public APlayerState,public IAbilitySystemInterface//接口
{
	GENERATED_BODY()

public:
	AAuraPlayerState();

	//重写接口里的获取技能组件函数，声明获取AttributeSet函数
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet*GetAttributeSet() const {return AttributeSet;}
protected:
	//创建AbilitySystemComponent和AttributeSet
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent>AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet>AttributeSet;

	
};
