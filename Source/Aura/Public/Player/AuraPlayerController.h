// Copyright JCS

#pragma once

#include "EnhancedInputSubsystems.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AuraPlayerController.generated.h"

//声明
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface;
/**
 * 
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AAuraPlayerController();//构造函数
	virtual void PlayerTick(float DeltaTime) override;//Tick
protected:
	virtual void BeginPlay() override;
	//设置输入
	virtual void SetupInputComponent() override;
private:
	//创建输入上下文
	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputMappingContext> AuraContext;
	
    //创建操作
	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputAction> MoveAction;

	void Move(const FInputActionValue& InputActionValue) ;//移动
	void CursorTrace();//追踪鼠标

	//查询上一帧和当前帧鼠标下的Actor
	IEnemyInterface*LastActor;
	IEnemyInterface*ThisActor;
	
};
