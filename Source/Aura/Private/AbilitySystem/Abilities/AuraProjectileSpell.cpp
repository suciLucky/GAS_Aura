// Copyright JCS


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"
#include "Player/AuraPlayerController.h"
#include "Interaction/EnemyInterface.h"
#include "Kismet/KismetMathLibrary.h"


void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	/**
	 *Self solution
	const FVector PlayerLocation = GetAvatarActorFromActorInfo()->GetActorLocation();
	AAuraPlayerController*PlayerController = Cast<AAuraPlayerController>(GetAvatarActorFromActorInfo()->GetInstigatorController());
	FHitResult CursorHit;
	PlayerController->GetHitResultUnderCursor(ECC_Visibility,false,CursorHit);
	if(Cast<IEnemyInterface>(CursorHit.GetActor()))
	{
		TargetActor=CursorHit.GetActor();
		const FVector TargetLocation = CursorHit.GetActor()->GetActorLocation();
		GetAvatarActorFromActorInfo()->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(PlayerLocation,TargetLocation));
	}
	*/	
}
void UAuraProjectileSpell::SpawnProjectile()
{
	//检查是否在服务器上调用
	const bool IsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if(!IsServer)return;

	//使用接口的获取插槽位置,在服务器上生成
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());
	if(CombatInterface)
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(SocketLocation);
		//TODO:Set Rotation
		/**
		 *Self solution
		if(TargetActor)
		{
			const FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(SocketLocation,TargetActor->GetActorLocation());
			SpawnTransform.SetRotation(Rotation.Quaternion());
		}
		*/
		
		AAuraProjectile*Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass,SpawnTransform,
			GetAvatarActorFromActorInfo(),Cast<APawn>(GetAvatarActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		//TODO:Give the projectile a GE spec for causing damage
		Projectile->FinishSpawning(SpawnTransform);
	}	
}
