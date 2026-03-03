// Copyright JCS


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Player/AuraPlayerState.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	//广播初始值
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	//将PlayerState的委托绑定到回调函数
	AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	AuraPlayerState->OnXPChangedDelegate.AddUObject(this,&UOverlayWidgetController::OnXPChanged);

	AuraPlayerState->OnLevelChangedDelegate.AddLambda(
	[this](int32 NewLevel)
	{
		OnPlayerLevelChangedDelegate.Broadcast(NewLevel);
	}
	);	
	
	const UAuraAttributeSet* AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);	
	//根据改变的属性将它的属性改变委托绑定到对应Lambda响应函数
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute())
	                      .AddLambda(
		                      [this](const FOnAttributeChangeData& Data)
		                      {
			                      OnHealthChanged.Broadcast(Data.NewValue);
		                      }
	                      );
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute())
	                      .AddLambda(
		                      [this](const FOnAttributeChangeData& Data)
		                      {
			                      OnMaxHealthChanged.Broadcast(Data.NewValue);
		                      }
	                      );
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetManaAttribute())
	                      .AddLambda(
		                      [this](const FOnAttributeChangeData& Data)
		                      {
			                      OnManaChanged.Broadcast(Data.NewValue);
		                      }
	                      );
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxManaAttribute())
	                      .AddLambda(
		                      [this](const FOnAttributeChangeData& Data)
		                      {
			                      OnMaxManaChanged.Broadcast(Data.NewValue);
		                      }
	                      );

	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		//绑定初始技能委托的回调函数
		if (AuraASC->bStartupAbilitiesGiven) OnInitializeStartupAbilities(AuraASC);
		else
		{
			AuraASC->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::OnInitializeStartupAbilities);
		}

		//使用Lambda表达式作为回调函数
		AuraASC->EffectAssetTags.AddLambda(
			[this](const FGameplayTagContainer& AssetTagContainer)
			{
				for (auto Tag : AssetTagContainer)
				{
					//寻找Message类型的Tag
					if (Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Message"))))
					{
						//使用该Tag从数据表找特定行，然后广播
						const FUIWidgetRow* Row = GetDatatableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
						MessageWidgetRowDelegate.Broadcast(*Row);
					}
				}
			});
	}
}

void UOverlayWidgetController::OnInitializeStartupAbilities(UAuraAbilitySystemComponent* AuraAbilitySystemComponent)
{
	if (!AuraAbilitySystemComponent->bStartupAbilitiesGiven) return;

	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this,AuraAbilitySystemComponent](const FGameplayAbilitySpec& AbilitySpec)
	{
		FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(
			AuraAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
		Info.InputTag = AuraAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
		AbilityInfoDelegate.Broadcast(Info);
	});
	AuraAbilitySystemComponent->ForEachAbility(BroadcastDelegate);
}

void UOverlayWidgetController::OnXPChanged(const int32 NewXP) const
{
	const AAuraPlayerState* AuraPlayerState = CastChecked<AAuraPlayerState>(PlayerState);
	const ULevelUpInfo* LevelUpInfo = AuraPlayerState->LevelUpInfo;
	checkf(LevelUpInfo,TEXT("Unabled to find LevelUpInfo,Pleade fill it out in BP_AuraPlayerState"));
	const int32 Level = LevelUpInfo->FindLevelForXP(NewXP); 

	if(Level <= LevelUpInfo->LevelUpInformation.Num() && Level > 0)
	{
		const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		const int32 PreviousLevelUpRequirement = LevelUpInfo->LevelUpInformation[Level-1].LevelUpRequirement;

		const int32 DeltaLevelRequirement = LevelUpRequirement - PreviousLevelUpRequirement;
		const int32 XPForThisLevel = NewXP - PreviousLevelUpRequirement;
		
		const float XPBarPercent = static_cast<float>(XPForThisLevel) / static_cast<float>(DeltaLevelRequirement);

		OnXPPercentChangedDelegate.Broadcast(XPBarPercent);
	}
}
