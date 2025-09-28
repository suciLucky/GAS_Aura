// Copyright JCS

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Player/AuraPlayerState.h"
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

protected:
	void InitAbilityActorInfo();
	
	
};
