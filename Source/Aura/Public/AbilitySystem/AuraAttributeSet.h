// Copyright JCS

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "GameplayEffectExtension.h"
#include "AbilitySystemComponent.h"
#include "AuraAttributeSet.generated.h"

//属性访问器宏
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

//存储和GE相关的变量
USTRUCT()
struct FEffectProperties
{
	GENERATED_BODY()

	FEffectProperties(){};

	FGameplayEffectContextHandle EffectContextHandle;

	UPROPERTY()
	UAbilitySystemComponent*SourceASC=nullptr;

	UPROPERTY()
	AActor*SourceAvatarActor=nullptr;

	UPROPERTY()
	AController*SourceController=nullptr;

	UPROPERTY()
    ACharacter*SourceCharacter=nullptr;

	UPROPERTY()
	UAbilitySystemComponent*TargetASC=nullptr;

	UPROPERTY()
	AActor*TargetAvatarActor=nullptr;

	UPROPERTY()
	AController*TargetController=nullptr;

	UPROPERTY()
	ACharacter*TargetCharacter=nullptr;	
};
/**
 * 
 */
UCLASS()
class AURA_API UAuraAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UAuraAttributeSet();//构造函数

	//注册需网络同步的属性并将服务器上的属性自动同步给所有客户端。
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//预构造属性改变函数，用于属性夹值
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	//GE生效后的执行函数，拥有所有相关数据
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	//创建可复制的变量，使用复制通知函数标记
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_Health,Category="Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,Health);//使用属性访问器宏来访问或修改属性

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_MaxHealth,Category="Vital Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,MaxHealth);

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_Mana,Category="Vital Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,Mana);

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_MaxMana,Category="Vital Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,MaxMana);

	//创建复制通知函数
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth)const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)const;

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana)const;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana)const;

private:

	//设置GE相关变量，在PostGameplayEffectExecute()里执行
	void static SetEffectProperties(const FGameplayEffectModCallbackData& Data,FEffectProperties& Props);
	
};
