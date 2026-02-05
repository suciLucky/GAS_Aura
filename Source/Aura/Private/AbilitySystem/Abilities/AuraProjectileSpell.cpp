// Copyright JCS


#include "AbilitySystem/Abilities/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "Actor/AuraProjectile.h"
#include "Player/AuraPlayerController.h"
#include "Kismet/KismetMathLibrary.h"


void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UAuraProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation,const FVector& SocketLocation,bool bOverridePitch ,float PitchOverride)
{
	//检查是否在服务器上调用,在服务器上生成
	const bool IsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if(!IsServer)return;
	
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(SocketLocation);

	//Set Rotation
	FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(SocketLocation,ProjectileTargetLocation);
	if(bOverridePitch)
	{
		Rotation.Pitch = PitchOverride;
	}
	SpawnTransform.SetRotation(Rotation.Quaternion());	

	//延迟生成
	AAuraProjectile*Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass,SpawnTransform,
		GetAvatarActorFromActorInfo(),Cast<APawn>(GetAvatarActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	//创建EffectContextHandle
	const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
	FGameplayEffectContextHandle EffectContextHandle = SourceASC->MakeEffectContext();
	EffectContextHandle.SetAbility(this);
	EffectContextHandle.AddSourceObject(Projectile);
	TArray<TWeakObjectPtr<AActor>> Actors;
	Actors.Add(Projectile);
	EffectContextHandle.AddActors(Actors);
	FHitResult HitResult;HitResult.Location=ProjectileTargetLocation;
	EffectContextHandle.AddHitResult(HitResult);
	//使用伤害GE创建SpecHandle并设置到Projectile Actor上	
	const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass,GetAbilityLevel(),EffectContextHandle);

	//使用伤害标签定义GE的修改幅度,幅度取伤害表格数据
	const FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	for (auto& Pair:DamageTypes)
	{
		const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,Pair.Key,ScaledDamage);
	}		
		
	Projectile->DamageEffectSpecHandle = SpecHandle;

	//结束生成
	Projectile->FinishSpawning(SpawnTransform);
}
