// Copyright JCS

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "AttributeInfo.generated.h"

//包含Attribute信息的结构体
USTRUCT(BlueprintType)
struct FAuraAttributeInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	FGameplayTag AttributeTag=FGameplayTag();

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	FText AttributeName=FText();

	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	FText AttributeDescription =FText();

	UPROPERTY(BlueprintReadOnly)
	float AttributeValue = 0.f;
	
};

/**
 * 
 */
UCLASS()
class AURA_API UAttributeInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	//通过标签在结构体数组中找到特定信息结构体
	FAuraAttributeInfo FindAttributeInfoForTag(const FGameplayTag& AttributeTag,const bool bLogNotFound=false) const;

	//存储不同Attribute的信息结构体
	UPROPERTY(BlueprintReadOnly,EditDefaultsOnly)
	TArray<FAuraAttributeInfo> AttributeInformation;
};
