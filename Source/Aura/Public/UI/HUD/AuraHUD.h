// Copyright JCS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "AuraHUD.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()
public:
	//已创建出来的Widget变量
	UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverlayWidget;

	//创建Overlay的Controller
	UOverlayWidgetController*GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	//初始化widget和controller
	void InitOverlay(APlayerController*PC,APlayerState*PS,UAbilitySystemComponent*ASC,UAttributeSet*AS);
	
private:
	//要在HUD中添加的Widget和其Controller
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	//已创建的有效的Controller变量
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
};
