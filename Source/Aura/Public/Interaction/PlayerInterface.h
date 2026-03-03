// Copyright JCS

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AURA_API IPlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//读取经验
	UFUNCTION(BlueprintNativeEvent)
	int32 GetXP() const;

	//升级
	UFUNCTION(BlueprintNativeEvent)
	void LevelUp();

	//增加经验
	UFUNCTION(BlueprintNativeEvent)
	void AddToXP(int32 XP);

	//增加等级
	UFUNCTION(BlueprintNativeEvent)
	void AddToPlayerLevel(int32 InPlayerLevel);

	//增加属性点
	UFUNCTION(BlueprintNativeEvent)
	void AddToAttributePoints(int32 InAttributePoints);

	//增加技能点
	UFUNCTION(BlueprintNativeEvent)
	void AddToSpellPoints(int32 InSpellPoints);

	//基于经验查找等级
	UFUNCTION(BlueprintNativeEvent)
	int32 FindLevelForXP(int32 InXP) const;

	//基于等级获取奖励属性点
	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePointsReward(int32 Level) const;

	//获取属性点
	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePoints() const;

	//基于等级获取奖励技能点
	UFUNCTION(BlueprintNativeEvent)
	int32 GetSpellPointsReward(int32 Level) const;

	//获取技能点
	UFUNCTION(BlueprintNativeEvent)
	int32 GetSpellPoints() const;
};
