// Copyright JCS

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AbilityInfo.generated.h"


USTRUCT(BlueprintType)
struct FAuraAbilityInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	FGameplayTag AbilityTag = FGameplayTag();

	UPROPERTY(BlueprintReadOnly)
	FGameplayTag InputTag = FGameplayTag();

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	TObjectPtr<const UTexture2D> Icon = nullptr;

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	TObjectPtr<const UMaterialInterface> BackgroundMaterial = nullptr;
};
/**
 * 
 */
UCLASS()
class AURA_API UAbilityInfo : public UDataAsset
{
	GENERATED_BODY()

public:
	
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly,Category="AbilityInformation")	
	TArray<FAuraAbilityInfo> AbilityInformation;

	
	FAuraAbilityInfo FindAbilityInfoForTag(const FGameplayTag& AbilityTag,bool bLogNotFound = false) const;
};
