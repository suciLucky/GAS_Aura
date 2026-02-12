// Copyright JCS


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "Aura/AuraLogChannels.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this,&UAuraAbilitySystemComponent::ClientEffectApplied);
}

//添加初始技能和初始输入标签
void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupAbilities)
{
	for (const auto AbilityClass:StartupAbilities)
	{
		FGameplayAbilitySpec AbilitySpec=FGameplayAbilitySpec(AbilityClass,1);
		if(UAuraGameplayAbility*AuraGameplayAbility=Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(AuraGameplayAbility->StartupInputTag);
			GiveAbility(AbilitySpec);
		}		
	}
	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast(this);
}

//激活匹配标签的技能并通知已按下
void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& InputTag)
{
	if(!InputTag.IsValid())return;
	for(auto& AbilitySpec:GetActivatableAbilities())
	{
		if(AbilitySpec.DynamicAbilityTags.HasTag(InputTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if(!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
	
}

//通知匹配标签的技能已被松开
void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& InputTag)
{
	if(!InputTag.IsValid())return;
	for(auto& AbilitySpec:GetActivatableAbilities())
	{
		if(AbilitySpec.DynamicAbilityTags.HasTag(InputTag))
		{
			AbilitySpecInputReleased(AbilitySpec);			
		}
	}
	
}

void UAuraAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
	//锁住当前循环的列表，防止技能列表动态变化
	FScopedAbilityListLock ActiveScopeLock(*this);
	for(auto Spec : GetActivatableAbilities())
	{
		if(Delegate.ExecuteIfBound(Spec))
		{
			UE_LOG(LogAura,Error,TEXT("Faild to execute delegate in %hs"),__FUNCTION__);
		}
	}
}

FGameplayTag UAuraAbilitySystemComponent::GetAbilityTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	if(AbilitySpec.Ability)
	{
		for(auto Tag: AbilitySpec.Ability.Get()->AbilityTags)
		{
			if(Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Abilities"))))
				return Tag;
		}
	}
	return FGameplayTag();
}

FGameplayTag UAuraAbilitySystemComponent::GetInputTagFromSpec(const FGameplayAbilitySpec& AbilitySpec)
{
	for(auto Tag : AbilitySpec.DynamicAbilityTags)
	{
		if(Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
			return Tag;
	}
	return FGameplayTag();
}

void UAuraAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();

	if(!bStartupAbilitiesGiven)
	{
		bStartupAbilitiesGiven = true;
		AbilitiesGivenDelegate.Broadcast(this);
	}
	
}

void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                                     const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)const
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);

	EffectAssetTags.Broadcast(TagContainer);
}
