// Copyright JCS

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AI/AuraAIController.h"
#include "Character/AuraCharacterBase.h"
#include "Components/WidgetComponent.h"
#include "Interaction/EnemyInterface.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraEnemy.generated.h"

class UBehaviorTree;
/**
 * 
 */
UCLASS()
class AURA_API AAuraEnemy : public AAuraCharacterBase,public IEnemyInterface//继承敌人接口
{
	GENERATED_BODY()
public:
	AAuraEnemy();

	virtual void PossessedBy(AController* NewController) override;
	/**Enemy接口函数*/
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	/** end Enemy接口函数*/

	/**Combat接口函数*/
	virtual int32 GetPlayerLevel() override;
	virtual void Die() override;
	virtual void SetCombatTarget_Implementation(AActor* InCombatTarget) override;
	virtual AActor* GetCombatTarget_Implementation() const override;
	/** end Combat接口函数*/

	//属性变化委托
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnHealthChanged;
	UPROPERTY(BlueprintAssignable)
	FOnAttributeChangedSignature OnMaxHealthChanged;

	//受击标签变化时触发的回调函数
	void HitReactTagChanged(const FGameplayTag CallbackTag,int32 NewCount);

	//处于受击状态
	UPROPERTY(BlueprintReadOnly,Category="Combat")
	bool bHitReacting = false;

	//基础行走速度
	UPROPERTY(EditAnywhere,Category="Combat")
	float BaseWalkSpeed = 250.f;

	//生命周期
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Combat")
	float EnemyLifeSpan = 2.f;

	//攻击目标
	UPROPERTY(BlueprintReadWrite,Category="Combat")
	TObjectPtr<AActor> CombatTarget;
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

	//行为树
	UPROPERTY(EditAnywhere,Category="AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;
	//AIController
	UPROPERTY()
	TObjectPtr<AAuraAIController> AuraAIController;
	
};
