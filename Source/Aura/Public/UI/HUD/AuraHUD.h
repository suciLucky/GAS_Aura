// Copyright JCS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/Widget/AuraUserWidget.h"
#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "UI/WidgetController/OverlayWidgetController.h"
#include "UI/WidgetController/SpellMenuWidgetController.h"
#include "AuraHUD.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()
public:
	//创建Overlay的Controller
	UOverlayWidgetController*GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	//创建AttributeMenu的Controller
	UAttributeMenuWidgetController*GetAttributeMenuWidgetController(const FWidgetControllerParams& WCParams);
	//创建SpellMenu的Controller
	USpellMenuWidgetController*GetSpellMenuWidgetController(const FWidgetControllerParams& WCParams);
	
	//初始化widget和其controller
	void InitOverlay(APlayerController*PC,APlayerState*PS,UAbilitySystemComponent*ASC,UAttributeSet*AS);
	
private:
	
	//要在HUD中添加的Widget,在蓝图中选择
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;
	//已创建出来的Widget变量
    UPROPERTY()
	TObjectPtr<UAuraUserWidget> OverlayWidget;
	
	//要添加的Controller，在蓝图中选择
	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<USpellMenuWidgetController> SpellMenuWidgetControllerClass;
	
	//已创建的有效的Controller变量
	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;
	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;
	UPROPERTY()
	TObjectPtr<USpellMenuWidgetController>SpellMenuWidgetController;
};
