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

	//重写接口的函数
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	
protected:
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;//初始化info

		
};
