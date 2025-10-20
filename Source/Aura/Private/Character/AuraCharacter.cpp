// Copyright JCS


#include "Character/AuraCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/AuraPlayerState.h"
#include "Player/AuraPlayerController.h"
#include "UI/HUD/AuraHUD.h"

AAuraCharacter::AAuraCharacter()
{
	
	GetCharacterMovement()->bOrientRotationToMovement=true;//使角色旋转朝向运动
	GetCharacterMovement()->RotationRate=FRotator(0.f,400.f,0.f);//旋转速率
	GetCharacterMovement()->bConstrainToPlane=true;//使运动维持在平面
	GetCharacterMovement()->bSnapToPlaneAtStart=true;//角色贴合地面

	//禁用控制器旋转角色
	bUseControllerRotationPitch=false;
	bUseControllerRotationRoll=false;
	bUseControllerRotationYaw=false;
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//Init Ability Actor Info for the Server
	InitAbilityActorInfo();

	//添加初始能力
	AddCharacterAbilities();
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	//Init Ability Actor Info for the Client
	InitAbilityActorInfo();	
}

int32 AAuraCharacter::GetPlayerLevel()
{
	const AAuraPlayerState*AuraPlayerState=GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState)
	return AuraPlayerState->GetPlayerLevel();
}

void AAuraCharacter::InitAbilityActorInfo()
{
	//初始化ASC的ActorInfo
	AAuraPlayerState*AuraPlayerState=GetPlayerState<AAuraPlayerState>();
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState,this);
	Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	
	//初始化Aura的组件和属性
	AbilitySystemComponent=AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet=AuraPlayerState->GetAttributeSet();

	//初始化HUD
	if(AAuraPlayerController*AuraPlayerController=Cast<AAuraPlayerController>(GetController()))
	{
		if(AAuraHUD*AuraHUD=Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD->InitOverlay(AuraPlayerController,AuraPlayerState,AbilitySystemComponent,AttributeSet);
		}		
	}
	//初始化属性
	InitializeDefaultAttributes();
}
