// Copyright JCS

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChanged,int32 /*StateValue*/)
/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerState : public APlayerState,public IAbilitySystemInterface//接口
{
	GENERATED_BODY()

public:
	AAuraPlayerState();

	//注册需要网络同步的变量
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	//重写接口里的获取技能组件函数，声明获取AttributeSet函数
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet*GetAttributeSet() const {return AttributeSet;}

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelUpInfo> LevelUpInfo;
	
	FOnPlayerStateChanged OnXPChangedDelegate;
	FOnPlayerStateChanged OnLevelChangedDelegate;

	FORCEINLINE int32 GetPlayerLevel() const {return Level;}
	FORCEINLINE int32 GetXP() const {return XP;}

	void AddToLevel(int32 InLevel);
	void SetLevel(int32 InLevel);
	void AddToXP(int32 InXP);
	void SetXP(int32 InXP);

	
protected:
	//创建AbilitySystemComponent和AttributeSet
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAbilitySystemComponent>AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet>AttributeSet;

private:

	//等级
	UPROPERTY(EditAnywhere,ReplicatedUsing=OnRep_Level)
	int32 Level=1;

	//经验
	UPROPERTY(EditAnywhere,ReplicatedUsing=OnRep_XP)
	int32 XP=0;

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);

	UFUNCTION()
	void OnRep_XP(int32 OldXP);

	
};
