// Copyright JCS

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AuraUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UAuraUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	//View.控件需要设置的Controller
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject*InWidgetController);

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UObject>WidgetController;

protected:
	//Controller已设置后调用
	UFUNCTION(BlueprintImplementableEvent)
	void WidgetControllerSet();
	
	
};
