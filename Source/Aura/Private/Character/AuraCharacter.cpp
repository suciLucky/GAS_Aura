// Copyright JCS


#include "Character/AuraCharacter.h"

#include <iostream>

#include "GameFramework/CharacterMovementComponent.h"

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
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	//Init Ability Actor Info for the Client
	InitAbilityActorInfo();	
}

void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState*AuraPlayerState=GetPlayerState<AAuraPlayerState>();
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState,this);
	
	//初始化Aura的组件和属性
	AbilitySystemComponent=AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet=AuraPlayerState->GetAttributeSet();

	
}
