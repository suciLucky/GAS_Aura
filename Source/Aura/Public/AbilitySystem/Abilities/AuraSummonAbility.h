// Copyright JCS

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "AuraSummonAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraSummonAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

	//获取召唤位置
	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetSpawnLocations();

	//召唤数量
	UPROPERTY(EditDefaultsOnly,Category="Summoning")
	int32 MinionNums = 5;

	//召唤类
	UPROPERTY(EditDefaultsOnly,Category="Summoning")
	TArray<TSubclassOf<APawn>> MinionClasses;

	//最小召唤距离
	UPROPERTY(EditDefaultsOnly,Category="Summoning")
	float MinSpawnDistance = 50.f;

	//最大召唤距离
	UPROPERTY(EditDefaultsOnly,Category="Summoning")
	float MaxSpawnDistance = 250.f;

	//生成分布角
	UPROPERTY(EditDefaultsOnly,Category="Summoning")
	float SpawnSpread = 90.f;
};
