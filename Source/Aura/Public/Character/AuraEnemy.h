// Copyright JCS

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase,public IEnemyInterface//继承敌人接口
{
	GENERATED_BODY()
public:
	AAuraEnemy();

	/**Enemy接口函数*/
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	/** end Enemy接口函数*/

	/**Combat接口函数*/
	virtual int32 GetPlayerLevel() override;
	/** end Combat接口函数*/

	
protected:
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;//初始化info

	//等级
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="Character Class Default")
	int32 Level=1;

		
};
