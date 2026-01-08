// Copyright JCS

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
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

//可以获取静态函数指针的通用模板，这里需要用来获取GetAttribute()的指针，例如GetStrengthAttribute();
template<class T>
using TStaticFuncPtr = typename TBaseStaticDelegateInstance<T,FDefaultDelegateUserPolicy>::FFuncPtr;

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

	//用来存储Tag映射GetAttribute()指针
	TMap<FGameplayTag,TStaticFuncPtr<FGameplayAttribute()>> TagToAttribute;

	//创建可复制的变量，使用复制通知函数标记（Vital Attributes）
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_Health,Category="Vital Attributes")
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,Health);//使用属性访问器宏来访问或修改属性
	
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_Mana,Category="Vital Attributes")
	FGameplayAttributeData Mana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,Mana);

	//Meta Attributes(对实际属性造成效果用到的中间属性，GE修改该属性)
	UPROPERTY(BlueprintReadOnly,Category="Meta Attributes")
	FGameplayAttributeData IncomingDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,IncomingDamage);

	//Primary Attributes
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_Strength,Category="Primary Attributes")
	FGameplayAttributeData Strength;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,Strength);

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_Intelligence,Category="Primary Attributes")
	FGameplayAttributeData Intelligence;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,Intelligence);

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_Resilience,Category="Primary Attributes")
	FGameplayAttributeData Resilience;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,Resilience);

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_Vigor,Category="Primary Attributes")
	FGameplayAttributeData Vigor;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,Vigor);

	//Secondary Attributes
	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_Armor,Category="Secondary Attributes")
	FGameplayAttributeData Armor;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,Armor);

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_ArmorPenetration,Category="Secondary Attributes")
	FGameplayAttributeData ArmorPenetration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,ArmorPenetration);

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_BlockChance,Category="Secondary Attributes")
	FGameplayAttributeData BlockChance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,BlockChance);

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_CriticalHitChance,Category="Secondary Attributes")
	FGameplayAttributeData CriticalHitChance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,CriticalHitChance);

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_CriticalHitDamage,Category="Secondary Attributes")
	FGameplayAttributeData CriticalHitDamage;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,CriticalHitDamage);

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_CriticalHitResistance,Category="Secondary Attributes")
	FGameplayAttributeData CriticalHitResistance;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,CriticalHitResistance);

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_HealthRegeneration,Category="Secondary Attributes")
	FGameplayAttributeData HealthRegeneration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,HealthRegeneration);

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_ManaRegeneration,Category="Secondary Attributes")
	FGameplayAttributeData ManaRegeneration;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,ManaRegeneration);

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_MaxHealth,Category="Vital Attributes")
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,MaxHealth);

	UPROPERTY(BlueprintReadOnly,ReplicatedUsing=OnRep_MaxMana,Category="Vital Attributes")
	FGameplayAttributeData MaxMana;
	ATTRIBUTE_ACCESSORS(UAuraAttributeSet,MaxMana);
	

	//创建复制通知函数
	//Vital Attributes
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldHealth)const;

	UFUNCTION()
	void OnRep_Mana(const FGameplayAttributeData& OldMana)const;

	//Primary Attributes
	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldStrength)const;

	UFUNCTION()
	void OnRep_Intelligence(const FGameplayAttributeData& OldIntelligence)const;

	UFUNCTION()
	void OnRep_Resilience(const FGameplayAttributeData& OldResilience)const;

	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldVigor)const;

	//Secondary Attributes
	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldArmor)const;

	UFUNCTION()
	void OnRep_ArmorPenetration(const FGameplayAttributeData& OldArmorPenetration)const;

	UFUNCTION()
	void OnRep_BlockChance(const FGameplayAttributeData& OldBlockChance)const;

	UFUNCTION()
	void OnRep_CriticalHitChance(const FGameplayAttributeData& OldCriticalHitChance)const;

	UFUNCTION()
	void OnRep_CriticalHitDamage(const FGameplayAttributeData& OldCriticalHitDamage)const;

	UFUNCTION()
	void OnRep_CriticalHitResistance(const FGameplayAttributeData& OldCriticalHitResistance)const;

	UFUNCTION()
	void OnRep_HealthRegeneration(const FGameplayAttributeData& OldHealthRegeneration)const;

	UFUNCTION()
	void OnRep_ManaRegeneration(const FGameplayAttributeData& OldManaRegeneration)const;

	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)const;

	UFUNCTION()
	void OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana)const;
private:

	//设置GE相关变量，在PostGameplayEffectExecute()里执行
	void static SetEffectProperties(const FGameplayEffectModCallbackData& Data,FEffectProperties& Props);
	
};





