// Copyright JCS


#include "UI/HUD/AuraHUD.h"



UOverlayWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if(OverlayWidgetController==nullptr)
	{
		OverlayWidgetController=NewObject<UOverlayWidgetController>(this,OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		//绑定对应的回调响应函数
		OverlayWidgetController->BindCallbacksToDependencies();
	}
	return OverlayWidgetController;
}

UAttributeMenuWidgetController* AAuraHUD::GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if(AttributeMenuWidgetController==nullptr)
	{
		AttributeMenuWidgetController=NewObject<UAttributeMenuWidgetController>(this,AttributeMenuWidgetControllerClass);
		AttributeMenuWidgetController->SetWidgetControllerParams(WCParams);
		AttributeMenuWidgetController->BindCallbacksToDependencies();
	}
	return AttributeMenuWidgetController;
}

USpellMenuWidgetController* AAuraHUD::GetSpellMenuWidgetController(const FWidgetControllerParams& WCParams)
{
	if(SpellMenuWidgetController==nullptr)
	{
		SpellMenuWidgetController=NewObject<USpellMenuWidgetController>(this,SpellMenuWidgetControllerClass);
		SpellMenuWidgetController->SetWidgetControllerParams(WCParams);
		SpellMenuWidgetController->BindCallbacksToDependencies();
	}
	return SpellMenuWidgetController;
}

void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass,TEXT("OverlayWidgetClass is uninitialized,please fill it out in BP_AuraHUD"));
	checkf(OverlayWidgetControllerClass,TEXT("OverlayWidgetControllerClass is uninitialized,please fill it out in BP_AuraHUD"));
		
	//创建Widget
 	UUserWidget*Widget=CreateWidget<UUserWidget>(GetWorld(),OverlayWidgetClass);
	OverlayWidget=Cast<UAuraUserWidget>(Widget);

	//创建controller并初始化
	const FWidgetControllerParams WCParams(PC,PS,ASC,AS);
	UOverlayWidgetController*WidgetController=GetOverlayWidgetController(WCParams);
	OverlayWidget->SetWidgetController(WidgetController);

	//全部就绪后广播初始值
	WidgetController->BroadcastInitialValues();
	
 	Widget->AddToViewport();//添加到屏幕
}
