// Copyright JCS

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelUpInfo.generated.h"

USTRUCT(BlueprintType)
struct FAuraLevelUpInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	int32 LevelUpRequirement = 0;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	int32 AttributePointAward = 1;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	int32 SpellPointAward = 1;
};
/**
 * 
 */
UCLASS()
class AURA_API ULevelUpInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly,Category="LevelUpInformation")	
	TArray<FAuraLevelUpInfo> LevelUpInformation;

	int32 FindLevelForXP(const int32& XP) const;
};
