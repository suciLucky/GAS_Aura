// Copyright JCS

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "Actor/AuraProjectile.h"
#include "AuraProjectileSpell.generated.h"

/**
 * 施法发射的技能
 */
UCLASS()
class AURA_API UAuraProjectileSpell : public UAuraGameplayAbility
{
	GENERATED_BODY()
protected:
	//激活技能
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	//要发射的发射物
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<AAuraProjectile> ProjectileClass;
	
	UFUNCTION(BlueprintCallable,Category="Projectile")
	void SpawnProjectile(const FVector& ProjectileTargetLocation);

	//蓝图中选择伤害效果类
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;
};
