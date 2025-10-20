// Copyright JCS

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;

UCLASS(Abstract)
class AURA_API AAuraCharacterBase : public ACharacter,public IAbilitySystemInterface,public ICombatInterface//接口
{
	GENERATED_BODY()

public:

	AAuraCharacterBase();
	//重写接口里的获取技能组件函数，声明获取AttributeSet函数
    virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet*GetAttributeSet() const {return AttributeSet;}

protected:

	virtual void BeginPlay() override;

//创建武器
	UPROPERTY(EditAnywhere,Category="Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;
	
	//创建AbilitySystemComponent和AttributeSet（给Enemy使用，在Enemy中定义）
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent>AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UAttributeSet>AttributeSet;

	virtual void InitAbilityActorInfo();//初始化info

	//用于初始化Vital属性值的GE
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributes;

	
	//用于初始化Primary属性值的GE
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributes;

	//用于初始化Secondary属性值的GE
	UPROPERTY(BlueprintReadOnly,EditAnywhere,Category="Attributes")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributes;

	//应用初始化属性GE的函数
	void InitializeDefaultAttributes() const;

	//应用GE到自身的函数
	void ApplyEffectToSelf(const TSubclassOf<UGameplayEffect>& GameplayEffectClass,const float& Level) const;

	void AddCharacterAbilities();
private:

	//初始Abilities列表
	UPROPERTY(EditAnywhere,Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> StartupAbilities;

};
