// Copyright JCS

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags,const FGameplayTagContainer& /*AssetTags*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FAbilitiesGiven,UAuraAbilitySystemComponent*);
DECLARE_DELEGATE_OneParam(FForEachAbility,const FGameplayAbilitySpec&);

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
public:
	//使用ASC的功能。例如绑定GE响应委托，或其他的功能
	void AbilityActorInfoSet();

	//广播AssetTags的委托
	FEffectAssetTags EffectAssetTags;

	//广播初始技能的委托
	FAbilitiesGiven AbilitiesGivenDelegate;

	//判断是否技能已就绪
	bool bStartupAbilitiesGiven = false;

	//添加初始技能和初始输入标签
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities);

	//按下、松开时触发的函数，接受InputTag
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

	void ForEachAbility(const FForEachAbility& Delegate);

	//从Spec获取该Ability的Tag
	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);
	//从Spec获取该Ability的InputTag
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec);

	//在服务器上加点更改属性值	
	void UpgradeAttribute(const FGameplayTag& AttributeTag);
	UFUNCTION(Server,Reliable)
	void ServerUpgradeAttribute(const FGameplayTag& AttributeTag);
protected:

	//GE生效时的Client RPC回调函数，通过多播委托绑定
	UFUNCTION(Client,Reliable)
	void ClientEffectApplied( UAbilitySystemComponent*AbilitySystemComponent, const FGameplayEffectSpec&EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)const;

	//在技能被复制时也触发广播
	virtual void OnRep_ActivateAbilities() override;
};
