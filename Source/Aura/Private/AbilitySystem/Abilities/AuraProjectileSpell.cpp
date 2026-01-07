// Copyright JCS


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Actor/AuraProjectile.h"
#include "Interaction/CombatInterface.h"
#include "Player/AuraPlayerController.h"
#include "Kismet/KismetMathLibrary.h"


void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
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

		//Set Rotation
		FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(SocketLocation,ProjectileTargetLocation);
		Rotation.Pitch=0.f;
		SpawnTransform.SetRotation(Rotation.Quaternion());

		//延迟生成
		AAuraProjectile*Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass,SpawnTransform,
			GetAvatarActorFromActorInfo(),Cast<APawn>(GetAvatarActorFromActorInfo()),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		//使用伤害GE创建SpecHandle并设置到Projectile Actor上
		const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
		const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass,GetAbilityLevel(),SourceASC->MakeEffectContext());
		Projectile->DamageEffectSpecHandle = SpecHandle;

		//结束生成
		Projectile->FinishSpawning(SpawnTransform);
	}	
}
