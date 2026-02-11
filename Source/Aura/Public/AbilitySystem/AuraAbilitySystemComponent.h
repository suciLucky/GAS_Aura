// Copyright JCS

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"


DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags,const FGameplayTagContainer& /*AssetTags*/)
DECLARE_MULTICAST_DELEGATE_OneParam(FAbilitiesGiven,UAuraAbilitySystemComponent*)

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

	//按下、松开时触发的函数，接受InputTag
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
protected:

	//GE生效时的Client RPC回调函数，通过多播委托绑定
	UFUNCTION(Client,Reliable)
	void ClientEffectApplied( UAbilitySystemComponent*AbilitySystemComponent, const FGameplayEffectSpec&EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)const;
	
};
