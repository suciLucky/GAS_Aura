// Copyright JCS

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/Widget/AuraUserWidget.h"
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
protected:
	virtual void BeginPlay() override;

private:

	//要在HUD中添加的Widget
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAuraUserWidget> OverlayWidgetClass;
};
