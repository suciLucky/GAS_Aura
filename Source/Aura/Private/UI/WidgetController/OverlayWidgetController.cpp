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

	if(UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		//绑定初始技能委托的回调函数
		if(AuraASC->bStartupAbilitiesGiven) OnInitializeStartupAbilities(AuraASC);
		else
		{
			AuraASC->AbilitiesGivenDelegate.AddUObject(this,&UOverlayWidgetController::OnInitializeStartupAbilities);
		}
		
		//使用Lambda表达式作为回调函数
        	AuraASC->EffectAssetTags.AddLambda(
            [this](const FGameplayTagContainer& AssetTagContainer)
            {
            	for(auto Tag:AssetTagContainer)
            	{
            		//寻找Message类型的Tag
            		if(Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Message"))))
            		{
            			//使用该Tag从数据表找特定行，然后广播
            			const FUIWidgetRow* Row=GetDatatableRowByTag<FUIWidgetRow>(MessageWidgetDataTable,Tag);
            			MessageWidgetRowDelegate.Broadcast(*Row);
            		}  		
            	
            	}
        	
            });	
	}	
	
}

void UOverlayWidgetController::OnInitializeStartupAbilities(UAuraAbilitySystemComponent* AuraAbilitySystemComponent)
{
	if(!AuraAbilitySystemComponent->bStartupAbilitiesGiven) return;

	
}
