// Copyright JCS

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Data/AttributeInfo.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttributeInfoSignature,const FAuraAttributeInfo&,Info);
/**
 * 
 */
UCLASS(BlueprintType,Blueprintable)
class AURA_API UAttributeMenuWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()

public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	UPROPERTY(BlueprintAssignable,Category="GAS|Attributes")
	FAttributeInfoSignature AttributeInfoDelegate;

	UPROPERTY(BlueprintAssignable,Category="GAS|AttributePoints")
	FOnPlayerStateChangedSignature AttributePointsChangedDelegate;

	UPROPERTY(BlueprintAssignable,Category="GAS|SpellPoints")
	FOnPlayerStateChangedSignature SpellPointsChangedDelegate;

	//加点更改属性值
	UFUNCTION(BlueprintCallable)
	void UpgradeAttribute(const FGameplayTag& AttributeTag);
	
protected:
	//蓝图中添加
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAttributeInfo> AttributeInfo;

private:
	//广播AttributeInfo
	void BroadcastAttributeInfo(const FGameplayTag& GameplayTag,const FGameplayAttribute& GameplayAttribute) const;
	
};
