// Copyright JCS

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "AuraCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase
{
	GENERATED_BODY()
public:
	AAuraCharacter();//构造函数

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void OnRep_Controller() override;

	/**Combat接口函数*/
	virtual int32 GetPlayerLevel() override;
	/** end Combat接口函数*/

protected:
	virtual void InitAbilityActorInfo() override;//初始化info
	
	
};
