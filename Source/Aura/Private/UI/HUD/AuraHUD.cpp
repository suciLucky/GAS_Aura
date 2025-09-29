// Copyright JCS


#include "UI/HUD/AuraHUD.h"

void AAuraHUD::BeginPlay()
{
	Super::BeginPlay();

	//创建Widget
	UUserWidget*Widget=CreateWidget<UUserWidget>(GetWorld(),OverlayWidgetClass);
	Widget->AddToViewport();//添加到屏幕
}
