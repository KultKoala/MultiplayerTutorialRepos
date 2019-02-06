// Fill out your copyright notice in the Description page of Project Settings.

#include "SWeapon.h"
#include "Camera/CameraComponent.h"
#include "Components/WidgetComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "MultiplayerTutorial.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"

static int32 DebugWeaponDrawing = 0;
FAutoConsoleVariableRef CVarDebugWeaponDrawing(
	TEXT("COOP.DebugWeapons"),
	DebugWeaponDrawing,
	TEXT("Draw Debug Lines For Weapons"),
	ECVF_Cheat);

// Sets default values
ASWeapon::ASWeapon()
{
	SkelMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkelMeshComp"));
	RootComponent = SkelMeshComp;

	MuzzleSocketName = "MuzzleSocket";
	DefaultDamage = 20.0f;
	FireRate = 600.0f;
	SetReplicates(true);
	NetUpdateFrequency = 66.0f;
	MinNetUpdateFrequency = 33.0f;
}


void ASWeapon::BeginPlay()
{
	Super::BeginPlay();
	TimeBetweenShots = 60.0 / FireRate;
}

void ASWeapon::BeginFire()
{
	float FirstDelay = FMath::Max(LastFireTime + TimeBetweenShots - GetWorld()->GetTimeSeconds(),0.0f);
	GetWorldTimerManager().SetTimer(TimerHandle, this, &ASWeapon::Fire, TimeBetweenShots, true, FirstDelay);
}

void ASWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(TimerHandle);
}

void ASWeapon::Fire()
{
	if (Role < ROLE_Authority) {
		ServerFire();
	}

	AActor* MyActor = GetOwner();
	if (MyActor)
	{
		FVector ViewLocation;
		FRotator ViewRotation;
		MyActor->GetActorEyesViewPoint(ViewLocation, ViewRotation);

		FHitResult OutHit;
		FVector ShotDirection = ViewRotation.Vector();
		FVector TraceEnd = ViewLocation + (ViewRotation.Vector() * 10000);
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(MyActor);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;
		QueryParams.bReturnPhysicalMaterial = true;

		FVector TracerEndPoint = TraceEnd;
		EPhysicalSurface SurfaceType = SurfaceType_Default;
		LastFireTime = GetWorld()->GetTimeSeconds();

		//Trace the world from pawn eyes to crosshair location
		if (GetWorld()->LineTraceSingleByChannel(OutHit,
			ViewLocation,
			TraceEnd,
			WEAPON_CHANNEL,
			QueryParams))
		{
			AActor* HitActor = OutHit.GetActor();

			SurfaceType = UPhysicalMaterial::DetermineSurfaceType(OutHit.PhysMaterial.Get());
			float AppliedDamage = DefaultDamage;

			if (SurfaceType == VULNERABLE_FLESH) {
				AppliedDamage *= 4;
			}
			
			UGameplayStatics::ApplyPointDamage(HitActor, AppliedDamage, ShotDirection, OutHit, MyActor->GetInstigatorController(), this, DamageType);
			PlayImpactEffects(SurfaceType,OutHit.ImpactPoint);
			TracerEndPoint = OutHit.ImpactPoint;

			if (DebugWeaponDrawing) {
				DrawDebugLine(GetWorld(), ViewLocation, TraceEnd, FColor::Cyan, false, 1.0f, 0, 1.0f);
			}
		}
		PlayFireEffects(TracerEndPoint);
		
		//get owner and play camera shake
		APawn* Owner = Cast<APawn>(GetOwner());
		if (Owner) {
			APlayerController *PC = Cast<APlayerController>(Owner->GetController());
			if (CameraShakeClass && PC) {
				PC->ClientPlayCameraShake(CameraShakeClass);
			}
		}

		if (Role == ROLE_Authority) {
			HitScanTrace.TraceEnd = TracerEndPoint;
			HitScanTrace.ImpactSurface = SurfaceType;
		}

	}
}

void ASWeapon::PlayFireEffects(const FVector &TracerEndPoint)
{

	FVector MuzzleLocation = SkelMeshComp->GetSocketLocation(MuzzleSocketName);

	if (MuzzleFlash) {
		UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, SkelMeshComp, MuzzleSocketName);
	}
	if (SmokeBeam) {
		UParticleSystemComponent* TraceComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SmokeBeam, MuzzleLocation);
		if (TraceComp) {
			TraceComp->SetVectorParameter("Target", TracerEndPoint);
		}
	}
}

void ASWeapon::PlayImpactEffects(EPhysicalSurface SurfaceType, FVector ImpactPoint)
{
	UParticleSystem * ImpactEffect = nullptr;


	switch (SurfaceType) {
	case DEFAULT_FLESH:
	case VULNERABLE_FLESH:
		ImpactEffect = FleshImpactEffect;
		break;
	default:
		ImpactEffect = DefaultImpactEffect;
		break;
	}

	FVector MuzzleLocation = SkelMeshComp->GetSocketLocation(MuzzleSocketName);
	FVector Direction = ImpactPoint - MuzzleLocation;
	Direction.Normalize();

	if (ImpactEffect) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, ImpactPoint, Direction.Rotation(), true);
	}
}

void ASWeapon::OnRep_HitScanTrace()
{
	//Play cosmetic FX
	PlayFireEffects(HitScanTrace.TraceEnd);
	PlayImpactEffects(HitScanTrace.ImpactSurface, HitScanTrace.TraceEnd);
}

void ASWeapon::ServerFire_Implementation()
{
	Fire();
}

bool ASWeapon::ServerFire_Validate()
{
	return true;
}

void ASWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ASWeapon, HitScanTrace, COND_SkipOwner);
}

