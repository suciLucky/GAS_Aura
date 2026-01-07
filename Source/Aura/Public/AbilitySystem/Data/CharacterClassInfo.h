// Copyright JCS

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"
//角色类型枚举，每种角色的数据有所不同
UENUM(BlueprintType)
enum class ECharacterClass:uint8
{
	Elementalist,
	Warrior,
	Ranger
};

//角色数据
USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly,Category="Class Defaults")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;
	
};
/**
 * 
 */
UCLASS()
class AURA_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()

public:

	//不同角色对应数据的Map
	UPROPERTY(EditDefaultsOnly,Category="Character Class Defaults")
	TMap<ECharacterClass,FCharacterClassDefaultInfo> CharacterClassInformation;

	//所有角色都有的数据
	UPROPERTY(EditDefaultsOnly,Category="Common Class Defaults")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;

	UPROPERTY(EditDefaultsOnly,Category="Common Class Defaults")
	TSubclassOf<UGameplayEffect> VitalAttributes;

	//通过角色种类在Map中搜寻数据
	FCharacterClassDefaultInfo GetClassDefaultInfo(const ECharacterClass CharacterClass);
	
};
