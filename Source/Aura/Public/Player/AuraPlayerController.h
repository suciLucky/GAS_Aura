// Copyright JCS

#pragma once

#include "EnhancedInputSubsystems.h"
#include "CoreMinimal.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "GameFramework/PlayerController.h"
#include "Input/AuraInputConfig.h"
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
	UPROPERTY(EditAnywhere,Category="Input")
	TObjectPtr<UInputAction> ShiftAction;

	void ShiftPressed(){bShiftKeyDown = true;}
	void ShiftReleased(){bShiftKeyDown = false;}
	bool bShiftKeyDown = false;

	void Move(const FInputActionValue& InputActionValue);//移动
	void CursorTrace();//追踪鼠标

	//查询上一帧和当前帧鼠标下的Actor
	IEnemyInterface*LastActor;
	IEnemyInterface*ThisActor;
	//获取鼠标下Actor 
	FHitResult CursorHit;

	//按下触发ASC对应的的函数
	void AbilityInputTagPressed(FGameplayTag InputTag);
	//松开触发ASC对应的的函数
	void AbilityInputTagReleased(FGameplayTag InputTag);
	//长按触发ASC对应的的函数
	void AbilityInputTagHeld(FGameplayTag InputTag);

	//带标签的IA资产列表
	UPROPERTY(EditDefaultsOnly,Category="Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	//复用的ASC
	UPROPERTY()
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;
	//在Controller中获取一次ASC
	UAuraAbilitySystemComponent*GetASC();

	//缓存的目的地点
	FVector CachedDestination=FVector();
	//持续跟随鼠标的时间
	float FollowTime=0.f;
	//短按鼠标的阈值
	float ShortPressThreshold=0.5f;
	//是否自动寻路
	bool bAutoRunning=false;
	//鼠标是否在索敌
	bool bTargeting=false;
	//到达目的地精确度误差
	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius=50.f;
	//寻路的路径样条
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;

	//自动寻路函数
	void AutoRun();
};
