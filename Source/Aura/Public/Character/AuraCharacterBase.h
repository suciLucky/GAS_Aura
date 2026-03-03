// Copyright JCS

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UNiagaraSystem;
class UAbilitySystemComponent;
class UAttributeSet;

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface //接口
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();
	//重写接口里的获取技能组件函数，声明获取AttributeSet函数
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	/** Combat Interface */
	virtual FVector GetCombatSocketLocation_Implementation(const FGameplayTag& SocketTag) override;
	virtual UAnimMontage* GetHitReactMontage_Implementation() override;
	virtual bool IsDead_Implementation() const override;
	virtual AActor* GetAvatar_Implementation() override;
	virtual void Die() override;
	virtual TArray<FTaggedMontage> GetAttackTaggedMontages_Implementation() override;
	virtual UNiagaraSystem* GetBloodEffect_Implementation() override;
	virtual FTaggedMontage GetTaggedMontageByTag_Implementation(const FGameplayTag& MontageTag) override;
	int32 GetMinionCount_Implementation() override;
	void IncrementMinionCount_Implementation(int32 Amount) override;
	virtual ECharacterClass GetCharacterClass_Implementation() override;
	/** end Combat Interface */

	//RPC处理所有端的死亡
	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();

	UPROPERTY(EditAnywhere, Category="Combat")
	TArray<FTaggedMontage> AttackTaggedMontages;

protected:
	virtual void BeginPlay() override;

	//创建武器
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	//武器插槽名
	UPROPERTY(EditAnywhere, Category="Combat")
	FName WeaponTipSocketName;
	//左手插槽名
	UPROPERTY(EditAnywhere, Category="Combat")
	FName LeftHandSocketName;
	//右手插槽名
	UPROPERTY(EditAnywhere, Category="Combat")
	FName RightHandSocketName;
	//尾巴插槽名
	UPROPERTY(EditAnywhere, Category="Combat")
	FName TailSocketName;

	//是否死亡
	bool bDead = false;

	//创建AbilitySystemComponent和AttributeSet（给Enemy使用，在Enemy中定义）
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	virtual void InitAbilityActorInfo(); //初始化info

	//用于初始化Vital属性值的GE
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;


	//用于初始化Primary属性值的GE
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	//用于初始化Secondary属性值的GE
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	//应用初始化属性GE的函数
	virtual void InitializeDefaultAttributes() const;

	//应用GE到自身的函数
	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass, const float& Level) const;

	void AddCharacterAbilities();

	//溶解材质
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> DissolveMaterialInstance;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UMaterialInstance> WeaponDissolveMaterialInstance;
	//溶解调用
	void Dissolve();
	//使用时间线在蓝图实施溶解
	UFUNCTION(BlueprintImplementableEvent)
	void StartDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);
	UFUNCTION(BlueprintImplementableEvent)
	void StartWeaponDissolveTimeline(UMaterialInstanceDynamic* DynamicMaterialInstance);

	//受击血液特效
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Combat")
	UNiagaraSystem* BloodEffect;

	//死亡音效
	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Combat")
	USoundBase* DeathSound;

	/* Minions */
	int32 MinionCount = 0;

	//角色种类
	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category="Character Class Default")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;

private:
	//初始Abilities列表
	UPROPERTY(EditAnywhere, Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

	//初始PassiveAbilities列表
	UPROPERTY(EditAnywhere, Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupPassiveAbilities;

	//受击蒙太奇
	UPROPERTY(EditAnywhere, Category="Combat")
	TObjectPtr<UAnimMontage> HitReactMontage;
};
