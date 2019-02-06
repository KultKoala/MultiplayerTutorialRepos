// Fill out your copyright notice in the Description page of Project Settings.

#include "SPickUpActor.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "SPowerUpActor.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "SCharacter.h"


// Sets default values
ASPickUpActor::ASPickUpActor()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetSphereRadius(75.0f, true);
	RootComponent = SphereComp;

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->SetRelativeRotation(FRotator(90, 0.0f, 0.0f));
	DecalComp->DecalSize = FVector(64, 75, 75);
	DecalComp->AttachToComponent(SphereComp,FAttachmentTransformRules::KeepRelativeTransform);

	RespawnTime = 15.0f;
	SetReplicates(true);

}

// Called when the game starts or when spawned
void ASPickUpActor::BeginPlay()
{
	Super::BeginPlay();
	if (Role == ROLE_Authority) {
		SpawnPowerUp();
	}
	
}

void ASPickUpActor::SpawnPowerUp()
{
	if (PowerUpActorClass) {
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		PowerUpActorInstance = GetWorld()->SpawnActor<ASPowerUpActor>(PowerUpActorClass, GetTransform(),SpawnParams);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Warning: Powerup is not assigned within a Pickup actor"))
		return;
	}
}


void ASPickUpActor::NotifyActorBeginOverlap(AActor * OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	//@TODO: Grant a powerup to player
	if (Role == ROLE_Authority && Cast<ASCharacter>(OtherActor) && PowerUpActorInstance) {
		PowerUpActorInstance->ActivatePowerUp();
		PowerUpActorInstance = nullptr;
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &ASPickUpActor::SpawnPowerUp, RespawnTime, false);
	}
	

	

}
