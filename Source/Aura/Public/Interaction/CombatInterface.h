// Copyright JCS

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "NiagaraSystem.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

//将Tag与多个信息对应，便于检索
USTRUCT(BlueprintType)
struct FTaggedMontage
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	UAnimMontage* AnimMontage = nullptr;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FGameplayTag MontageTag;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	FGameplayTag SocketTag;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
	USoundBase* ImpactSound = nullptr;
	
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI,BlueprintType)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	//获取等级
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	int32 GetPlayerLevel();

	//获取插槽位置
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	FVector GetCombatSocketLocation(const FGameplayTag& SocketTag);
	
	//更新朝向目标位置
	UFUNCTION(BlueprintImplementableEvent,BlueprintCallable)
	void UpdateFacingTarget(const FVector& Target);

	//获取受击蒙太奇
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	UAnimMontage* GetHitReactMontage();

	virtual void Die() = 0;	

	//获取死亡状态
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	bool IsDead() const;

	//获取化身Actor
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	AActor* GetAvatar();

	//获取带Tag的蒙太奇数组
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	TArray<FTaggedMontage> GetAttackTaggedMontages();

	//获取受击血液特效
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	UNiagaraSystem* GetBloodEffect();

	//通过MontageTag获取结构
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	FTaggedMontage GetTaggedMontageByTag(const FGameplayTag& MontageTag);

	//获取仆从数量
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	int32 GetMinionCount();

	//设置仆从数量
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	void IncrementMinionCount(int32 Amount);

	//获取角色类型
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable)
	ECharacterClass GetCharacterClass();
};
