// Copyright JCS


#include "Player/AuraPlayerController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;//可复制
	LastActor=ThisActor=nullptr;
	//构造寻路样条
	Spline=CreateDefaultSubobject<USplineComponent>("Spline");
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	//每帧追踪鼠标下的Actor
	CursorTrace();

	//自动寻路
	AutoRun();
	
}
void AAuraPlayerController::AutoRun()
{
	if(!bAutoRunning)return;
	if(APawn*ControlledPawn=GetPawn())
	{
		const FVector LocationOnSpline=Spline->FindLocationClosestToWorldLocation(
			ControlledPawn->GetActorLocation(),ESplineCoordinateSpace::World);
		const FVector Direction=Spline->FindDirectionClosestToWorldLocation(LocationOnSpline,ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);

		const float DistanceToDestination=(CachedDestination-LocationOnSpline).Length();
		if(DistanceToDestination<=AutoRunAcceptanceRadius)
		{
			bAutoRunning=false;
		}
	}
}
void AAuraPlayerController::CursorTrace()
{
	//获取鼠标下Actor 
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility,false,CursorHit);
	if(!CursorHit.bBlockingHit) return;//没命中不执行

	LastActor=ThisActor;//初始化俩帧Actor
	ThisActor=Cast<IEnemyInterface>(CursorHit.GetActor()); //查询命中的Actor是否继承（实现）EnemyInterface
	
	/**讨论上一帧和当前帧ACtor情况
	 *情况一：LastActor=ThisActor=NULL;--不进行操作
	 *情况二：LastActor=NULL,ThisActor有效；--需要HighLight()ThisActor；
	 *情况三：LastActor有效而ThisActor无效；--需要UnHighLight()LastActor；
	 *情况四：两者都有效但是两者不是同一个，意味着鼠标从上一个有效Actor移向下一个有效Actor；--需要HighLight()ThisActor而UnHighLight()LastActor；
	 *情况五：两者都有效且相等，说明鼠标悬停在某一有效Actor上；--前几种情况已经设置该Actor，所以不进行操作；
	 * */

	if(LastActor == nullptr)
	{
		if(ThisActor!=nullptr) ThisActor->HighlightActor();//情况二
		else{}//情况一
	}
	else //LastActor有效
	{
		if(ThisActor==nullptr) LastActor->UnHighlightActor();//情况三
		else 
		{
			if(LastActor!=ThisActor)//情况四
			{
				ThisActor->HighlightActor();
                LastActor->UnHighlightActor();
			}
			else{} //情况五			
		}		
	}	
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if(InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		//根据目标Actor是否有效设置索敌变量
		bTargeting=ThisActor?true:false;
        bAutoRunning=false;
	}
	
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	//不是鼠标左键的响应
	if(!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if(GetASC())GetASC()->AbilityInputTagReleased(InputTag);
		return;
	}
	//是鼠标左键但是在索敌的响应
	if(bTargeting)
	{
		if(GetASC())GetASC()->AbilityInputTagReleased(InputTag);
	}
	//寻路结束
	else
	{
		APawn*ControlledPawn=GetPawn();
		if(FollowTime<=ShortPressThreshold&&ControlledPawn)
		{
			if(UNavigationPath*NavPath=UNavigationSystemV1::FindPathToLocationSynchronously
				(this,ControlledPawn->GetActorLocation(),CachedDestination))
			{
				Spline->ClearSplinePoints();
				for(const auto&PointLocation:NavPath->PathPoints)
				{
					Spline->AddSplinePoint(PointLocation,ESplineCoordinateSpace::World);
					DrawDebugSphere(GetWorld(),PointLocation,5.f,8.f,FColor::Green,false,5.f);
				}
				CachedDestination=NavPath->PathPoints.Last();
				bAutoRunning=true;
			}
		}
		FollowTime=0.f;
		bTargeting=false;
	}

}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	//不是鼠标左键的响应
	if(!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if(GetASC())GetASC()->AbilityInputTagHeld(InputTag);
		return;
	}
	//是鼠标左键但是在索敌的响应
	if(bTargeting)
	{
		if(GetASC())GetASC()->AbilityInputTagHeld(InputTag);
	}
	//寻路响应
	else
	{
		FollowTime+=GetWorld()->GetDeltaSeconds();
		FHitResult Hit;
		if(GetHitResultUnderCursor(ECC_Visibility,false,Hit))
		{
			CachedDestination=Hit.ImpactPoint;
		}
		if(APawn*ControlledPawn=GetPawn())
		{
			const FVector WorldDirection= (CachedDestination-ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
		}
	}
	
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if(AuraAbilitySystemComponent==nullptr)
	{		
		AuraAbilitySystemComponent=Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
	}
	return AuraAbilitySystemComponent;
}



void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);//调试模式，断言
	
    //获得增强输入本地玩家子系统并设置映射上下文
	UEnhancedInputLocalPlayerSubsystem*Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if(Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext,0);
	}
	
	//设置鼠标光标
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	
    //设置输入模式为游戏和UI
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);	
}

void AAuraPlayerController::SetupInputComponent()
{
	//将输入组件转化为增强输入组件、绑定输入操作
	Super::SetupInputComponent();
	UAuraInputComponent*AuraInputComponent=CastChecked<UAuraInputComponent>(InputComponent);
	AuraInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered,this,&AAuraPlayerController::Move);

	AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed,
	                                       &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
	
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue) 
{
	const FVector2d InputAxisVector=InputActionValue.Get<FVector2d>();
	const FRotator Rotation=GetControlRotation();//获取了控制器当前的旋转值。在游戏中，这通常就等同于玩家摄像机的朝向。	
	const FRotator YawRotation(0.f,Rotation.Yaw,0.f);//确保我们计算出的移动方向永远是水平的

	//提取向前方向向量和向右方向向量
	const FVector ForwardDirection=FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection=FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	//为角色添加移动输入
	if(APawn *ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection,InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection,InputAxisVector.X);		
	}
}


