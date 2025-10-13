// Copyright JCS


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UAuraAttributeSet *AuraAttributeSet=CastChecked<UAuraAttributeSet>(AttributeSet);
	//广播初始值
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
	
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UAuraAttributeSet *AuraAttributeSet=CastChecked<UAuraAttributeSet>(AttributeSet);

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

	//使用Lambda表达式作为回调函数
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
    [this](const FGameplayTagContainer& AssetTagContainer)
    {
    	for(auto Tag:AssetTagContainer)
    	{
    		//寻找Message类型的Tag
    		FGameplayTag MessageTag=FGameplayTag::RequestGameplayTag(FName("Message"));
    		if(Tag.MatchesTag(MessageTag))
    		{
    			//使用该Tag从数据表找特定行，然后广播
    			const FUIWidgetRow* Row=GetDatatableRowByTag<FUIWidgetRow>(MessageWidgetDataTable,Tag);
    			MessageWidgetRowDelegate.Broadcast(*Row);
    		}  		
    	
    	}
	
    });	
}
