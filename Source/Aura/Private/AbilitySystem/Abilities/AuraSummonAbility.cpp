// Copyright JCS


#include "AbilitySystem/Abilities/AuraSummonAbility.h"



TArray<FVector> UAuraSummonAbility::GetSpawnLocations()
{
	const FVector AvatarForward  = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector AvatarLocation  = GetAvatarActorFromActorInfo()->GetActorLocation();
	const float DeltaSpread = SpawnSpread / MinionNums;
	
	TArray<FVector> Locations;

	const FVector LeftOfSpread = AvatarForward.RotateAngleAxis(-SpawnSpread/2.f,FVector::UpVector);
	for(int i = 0;i<MinionNums;++i)
	{
		const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread*i,FVector::UpVector);
		FVector Location = AvatarLocation+Direction*FMath::FRandRange(MinSpawnDistance,MaxSpawnDistance);
		FHitResult Hit;
		GetWorld()->LineTraceSingleByChannel(Hit,Location+FVector(0.f,0.f,400.f),Location-FVector(0.f,0.f,400.f),ECC_Visibility);
		if(Hit.bBlockingHit)Location = Hit.ImpactPoint;		
		Locations.Add(Location);		
	}	
	return Locations;
}

TSubclassOf<APawn> UAuraSummonAbility::GetRandomMinionClass()
{
	const int32 Selection = FMath::RandRange(0,MinionClasses.Num()-1);	
	return MinionClasses[Selection];
}
