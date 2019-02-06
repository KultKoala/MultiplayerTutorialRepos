// Fill out your copyright notice in the Description page of Project Settings.

#include "SGrenade.h"
#include "TimerManager.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
ASGrenade::ASGrenade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;

	ExplosionMeshComp = CreateDefaultSubobject<USphereComponent>(TEXT("ExplodeMeshComp"));
	ExplosionMeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ExplosionMeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ExplosionMeshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Overlap);
	ExplosionMeshComp->SetupAttachment(MeshComp);

	ProjectileComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileComp->InitialSpeed = 1000.0;

	Damage = 80.0f;
}

// Called when the game starts or when spawned
void ASGrenade::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(TimerHandle, 1.0, false);

}

void ASGrenade::Explode()
{
	if (ExplosionEffect) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation(), GetActorRotation(), true);
	}

	if (ExplosionMeshComp) {
		TArray<AActor *> OverlappingActors;
		ExplosionMeshComp->GetOverlappingActors(OverlappingActors);

		for (auto It = OverlappingActors.CreateConstIterator(); It; ++It) {
			if (It) {
				FHitResult Hit;
				UE_LOG(LogTemp, Warning, TEXT("Applying Force"));
				UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), ExplosionMeshComp->GetScaledSphereRadius(), TSubclassOf<UDamageType>(), TArray<AActor*>(), this, GetInstigatorController(),true);
			}
		}
	}
	Destroy();
}

// Called every frame
void ASGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!GetWorldTimerManager().IsTimerActive(TimerHandle)) {
		Explode();
	}

}

