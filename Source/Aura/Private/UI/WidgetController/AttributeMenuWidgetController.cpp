// Copyright JCS


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AuraGameplayTags.h"

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet*AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo);
	//获取AS里创建的Map进行广播
	for(auto& Pair:AS->TagToAttribute)
	{
		BroadcastAttributeInfo(Pair.Key,Pair.Value());
	}
	
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	UAuraAttributeSet*AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AttributeInfo);
	//属性改变时继续广播
	for(auto& Pair:AS->TagToAttribute)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this,Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key,Pair.Value());
			}
		);
	}
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& GameplayTag,
	const FGameplayAttribute& GameplayAttribute) const
{
	FAuraAttributeInfo Info=AttributeInfo->FindAttributeInfoForTag(GameplayTag);
	Info.AttributeValue=GameplayAttribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
