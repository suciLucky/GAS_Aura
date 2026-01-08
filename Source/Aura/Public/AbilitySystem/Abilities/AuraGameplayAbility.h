// Copyright JCS

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AuraGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly,Category="Input")
	FGameplayTag StartupInputTag;

	//技能通用的基础伤害，可用表格控制
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly,Category="Damage")
	FScalableFloat Damage;
};
