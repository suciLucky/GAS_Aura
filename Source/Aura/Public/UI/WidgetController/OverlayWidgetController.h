// Copyright JCS

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "OverlayWidgetController.generated.h"

//显示信息的UI的结构体
USTRUCT(BlueprintType)
struct FUIWidgetRow:public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FGameplayTag MessageTag=FGameplayTag();

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FText Message=FText();

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TSubclassOf<UAuraUserWidget> MessageWidget;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	UTexture2D*Image=nullptr;
	
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature,float,NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature,FUIWidgetRow,Row);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSignature,const FAuraAbilityInfo&,Info);

/**
 * 
 */
UCLASS(BlueprintType,Blueprintable)
class AURA_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;

	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnAttributeChangedSignature OnHealthChanged;
	
	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnAttributeChangedSignature OnManaChanged;
	
	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FOnAttributeChangedSignature OnMaxManaChanged;
	
	UPROPERTY(BlueprintAssignable,Category="GAS|Messages")
	FMessageWidgetRowSignature MessageWidgetRowDelegate;

	UPROPERTY(BlueprintAssignable,Category="GAS|Messages")
	FAbilityInfoSignature AbilityInfoDelegate;

	UPROPERTY(BlueprintAssignable,Category="GAS|XP")
	FOnAttributeChangedSignature OnXPPercentChangedDelegate;
	
	virtual void BindCallbacksToDependencies() override;
protected:

	//显示信息UI要使用的数据表
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable;

	//UI里的技能信息
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Widget Data")
	TObjectPtr<UAbilityInfo> AbilityInfo;

	//通过Tag获取任意数据表格行的模板函数
	template<typename T>
	T* GetDatatableRowByTag(UDataTable*DataTable,const FGameplayTag& Tag);

	//初始技能委托绑定的回调函数
	void OnInitializeStartupAbilities(UAuraAbilitySystemComponent* AuraAbilitySystemComponent);

	void OnXPChanged(const int32 NewXP)const;
};

template <typename T>
T* UOverlayWidgetController::GetDatatableRowByTag(UDataTable* DataTable, const FGameplayTag& Tag)
{
	return	DataTable->FindRow<T>(Tag.GetTagName(),TEXT(""));	
}
