// Copyright JCS


#include "Character/AuraCharacter.h"

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
