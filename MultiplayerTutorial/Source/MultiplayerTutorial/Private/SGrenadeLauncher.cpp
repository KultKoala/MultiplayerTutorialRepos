// Fill out your copyright notice in the Description page of Project Settings.

#include "SGrenadeLauncher.h"
#include "Components/SkeletalMeshComponent.h"

void ASGrenadeLauncher::Fire() {

	UE_LOG(LogTemp, Warning, TEXT("Grenade Launcher is launching"));
	//Get where player is looking
	AActor* MyActor = GetOwner();
	if (MyActor)
	{
		FVector ViewLocation;
		FRotator ViewRotation;
		MyActor->GetActorEyesViewPoint(ViewLocation, ViewRotation);

		FHitResult OutHit;
		FVector ShotDirection = ViewRotation.Vector();
		FVector TraceEnd = ViewLocation + (ViewRotation.Vector() * 10000);


		//Spawn Grenade at Muzzle Location
		FVector MuzzleLocation = SkelMeshComp->GetSocketLocation(MuzzleSocketName);

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		if (MuzzleFlash) {
			AActor * Projectile;
			Projectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, MuzzleLocation, ViewRotation, Params);
			if (Projectile) {
				//UE_LOG(LogTemp, Warning, TEXT("Pushing Grenade"));
				//UPrimitiveComponent * RootComp = Cast<UPrimitiveComponent>(Projectile->GetRootComponent());
				//RootComp->AddForce(ShotDirection * 10000);
			}

		}
	}

	//Push to where player is looking


}




