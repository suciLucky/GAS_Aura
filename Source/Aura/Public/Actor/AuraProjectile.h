// Copyright JCS

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "NiagaraSystem.h"
#include "AuraProjectile.generated.h"


UCLASS()
class AURA_API AAuraProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAuraProjectile();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent>ProjectileMovementComponent;

	//伤害GE的SpecHandle，在施法能力中设置
	UPROPERTY(BlueprintReadWrite,meta=(ExposeOnSpawn = true))
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	//发射物碰撞体重叠函数
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent*OverlappedComponent,AActor*OtherActor,
		UPrimitiveComponent*OtherComp,int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult);

private:
	//生命周期
	UPROPERTY(EditDefaultsOnly)
	float LifeSpan = 15.f;
	//是否碰撞
	bool bHit = false;
	//碰撞体
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;
	//碰撞特效
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ImpactEffect;
	//碰撞音效
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ImpactSound;
	//飞行音效
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> FlySound;

	UPROPERTY()
	TObjectPtr<UAudioComponent> FlySoundComp;
};
