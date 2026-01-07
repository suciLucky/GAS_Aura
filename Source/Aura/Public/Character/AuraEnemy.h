// Copyright JCS

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Character/AuraCharacterBase.h"
#include "Components/WidgetComponent.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
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

	//属性变化委托
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;
	
protected:
	virtual void BeginPlay() override;

	virtual void InitAbilityActorInfo() override;//初始化info

	//初始化角色数据
	virtual void InitializeDefaultAttributes() const override;

	//等级
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="Character Class Default")
	int32 Level=1;
	//角色种类
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="Character Class Default")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

	//敌人血条
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> EnemyHealthBar;

	

	
};
