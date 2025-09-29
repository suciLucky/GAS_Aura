// Copyright JCS


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AuraAttributeSet.h"


AAuraEffectActor::AAuraEffectActor()
{

	PrimaryActorTick.bCanEverTick = false;

	Mesh=CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);	

	Sphere=CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(RootComponent);

}

void AAuraEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//TODO:Change This to Apply GE.For Now,Using const_cast as a hack!
	if(IAbilitySystemInterface*ACSInterface=Cast<IAbilitySystemInterface>(OtherActor))
	{
		const UAuraAttributeSet*AuraAttributeSet=Cast<UAuraAttributeSet>(ACSInterface->GetAbilitySystemComponent()->GetAttributeSet(UAuraAttributeSet::StaticClass()));
		UAuraAttributeSet*MutableAuraAttributeSet=const_cast<UAuraAttributeSet*>(AuraAttributeSet);
		MutableAuraAttributeSet->SetHealth(MutableAuraAttributeSet->GetHealth()+25.f);
		Destroy();
	}
	
	
}

void AAuraEffectActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}


void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();

	//绑定这些事件
	Sphere->OnComponentBeginOverlap.AddDynamic(this,&AAuraEffectActor::OnOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this,&AAuraEffectActor::EndOverlap);
	
	
}



