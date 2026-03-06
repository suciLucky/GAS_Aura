// Copyright JCS


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AuraGameplayTags.h"
#include "Player/AuraPlayerState.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet*AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo);
	//获取AS里创建的Map进行广播
	for(auto& Pair:AS->TagToAttribute)
	{
		BroadcastAttributeInfo(Pair.Key,Pair.Value());
	}

	AttributePointsChangedDelegate.Broadcast(GetAuraPS()->GetAttributePoints());
	SpellPointsChangedDelegate.Broadcast(GetAuraPS()->GetSpellPoints());	
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{	
	check(AttributeInfo);
	//属性改变时继续广播
	for(auto& Pair:GetAuraAS()->TagToAttribute)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this,Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key,Pair.Value());
			}
		);
	}

	//将PlayerState的委托绑定到回调函数
	GetAuraPS()->OnAttributePointsChangedDelegate.AddLambda(
	[this](int32 NewAttributePoints)
	{
		AttributePointsChangedDelegate.Broadcast(NewAttributePoints);
	}
	);
	GetAuraPS()->OnSpellPointsChangedDelegate.AddLambda(
	[this](int32 NewSpellPoints)
	{
		SpellPointsChangedDelegate.Broadcast(NewSpellPoints);
	}
	);
}

void UAttributeMenuWidgetController::UpgradeAttribute(const FGameplayTag& AttributeTag)
{
	GetAuraASC()->UpgradeAttribute(AttributeTag);
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& GameplayTag,
                                                            const FGameplayAttribute& GameplayAttribute) const
{
	FAuraAttributeInfo Info=AttributeInfo->FindAttributeInfoForTag(GameplayTag);
	Info.AttributeValue=GameplayAttribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
