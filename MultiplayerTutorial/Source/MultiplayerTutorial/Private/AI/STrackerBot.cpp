// Fill out your copyright notice in the Description page of Project Settings.

#include "STrackerBot.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "AI/Navigation/NavigationSystem.h"
#include "AI/Navigation/NavigationPath.h"
#include "GameFramework/Character.h"
#include "SHealthComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Particles/ParticleSystem.h"
#include "DrawDebugHelpers.h"
#include "Components/SphereComponent.h"
#include "TimerManager.h"
#include "Sound/SoundCue.h"


// Sets default values
ASTrackerBot::ASTrackerBot()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCanEverAffectNavigation(false);
	MeshComp->SetSimulatePhysics(true);
	RootComponent = MeshComp;

	HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComp"));
	HealthComp->OnHealthChanged.AddDynamic(this, &ASTrackerBot::OnHealthChanged);

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComp->SetSphereRadius(DamageRadius);
	SphereComp->AttachToComponent(RootComponent,FAttachmentTransformRules::KeepRelativeTransform);

	MovementForce = 1000.0f;
	RequiredDistanceToTarget = 100.0f;
	bUseVelocityChange = true;
	BaseDamage = 50.0f;
	DamageRadius = 400.0f;

}

// Called when the game starts or when spawned
void ASTrackerBot::BeginPlay()
{
	Super::BeginPlay();
	if (Role == ROLE_Authority) {
		NextPoint = GetNextPathPoint();
	}
}

FVector ASTrackerBot::GetNextPathPoint()
{
	//Quick way to get player location. Will not scale to multiplayer
	ACharacter* PlayerPawn = UGameplayStatics::GetPlayerCharacter(this, 0);

	//Get navigation path to player
	if (Cast<AActor>(PlayerPawn)) {
		UNavigationPath* NavPath = UNavigationSystem::FindPathToActorSynchronously(this,GetActorLocation(), Cast<AActor>(PlayerPawn));
		//Get the next point to player
		if (NavPath  && (NavPath->PathPoints).Num() > 1) {
			return (NavPath->PathPoints[1]);
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Cannot find player pawn"))
	}

	//If failure return actor location
	return GetActorLocation();
}

void ASTrackerBot::MoveTowardsNextPoint()
{
	FVector ForceDirection = NextPoint - GetActorLocation();
	ForceDirection.Normalize();
	ForceDirection *= MovementForce;
	MeshComp->AddForce(ForceDirection,NAME_None,bUseVelocityChange);
}

void ASTrackerBot::HurtSelf()
{
	UGameplayStatics::ApplyDamage(this, 20.0f, GetInstigatorController(), this,nullptr);
}

void ASTrackerBot::SelfDestruct()
{
	//stops from being called more than twice
	if (bDied) {
		return;
	}

	bDied = true;

	//draw to see damage radius
	DrawDebugSphere(GetWorld(), GetActorLocation(), DamageRadius, 16, FColor::Green, false, 10.0f);

	//play explosion effect
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BotExplosion, GetActorLocation(), GetActorRotation(), true);
	UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());
	


	//apply damage only on server
	if (Role == ROLE_Authority) {
		//damage surrounding actors
		TArray<AActor*>Ignore;
		Ignore.Add(this);

		UGameplayStatics::ApplyRadialDamage(this, BaseDamage, GetActorLocation(), DamageRadius, nullptr, Ignore, this, GetInstigatorController(), true);

		Destroy();
	}
}

// Called every frame
void ASTrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float DistanceToTarget = (NextPoint - GetActorLocation()).Size();

	if (Role = ROLE_Authority) {
		

		if (DistanceToTarget <= RequiredDistanceToTarget) {
			NextPoint = GetNextPathPoint();
		}
		else {
			MoveTowardsNextPoint();
		}
	}

}

void ASTrackerBot::NotifyActorBeginOverlap(AActor * OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (Role == ROLE_Authority) {
		ACharacter *PlayerCharacter = Cast<ACharacter>(OtherActor);
		if (PlayerCharacter) {
			GetWorldTimerManager().SetTimer(TimerHandle_DamageSelf, this, &ASTrackerBot::HurtSelf, 0.5f, true, 0.0f);
		}
	}

	UGameplayStatics::SpawnSoundAttached(SelfDestructSound, RootComponent);
}

void ASTrackerBot::OnHealthChanged(USHealthComponent * HComp, float Health, float HealthDelta, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("Health %s of %s"), *FString::SanitizeFloat(Health), *GetName())

	//get dynamic material if not gotten already
	if (DynamicMat == nullptr){
		DynamicMat = MeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, MeshComp->GetMaterial(0));
	}
	
	//if material is gotten, access parameter in material blueprint that sets material to flash
	if (DynamicMat) {
		DynamicMat->SetScalarParameterValue("LastTimeDamageTaken", GetWorld()->GetTimeSeconds());
	}
	
	//if health is below a certain point, trigger boole
	if (Health <= 0.0f) {
		SelfDestruct();
	}

}


