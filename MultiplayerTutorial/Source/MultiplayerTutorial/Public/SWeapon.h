// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWeapon.generated.h"

class USkeletalMeshComponent;
class UDamageType;
class UParticleSystem;
class UCameraShake;

//Contains information about a hit scan weapon line trace
USTRUCT()
struct FHitScanTrace 
{
	GENERATED_BODY()
public:

	UPROPERTY()
	TEnumAsByte<EPhysicalSurface> ImpactSurface;

	UPROPERTY()
	FVector_NetQuantize TraceEnd;

};

UCLASS()
class MULTIPLAYERTUTORIAL_API ASWeapon : public AActor
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	ASWeapon();
	void BeginPlay() override;
	void BeginFire();
	void StopFire();

protected:

	virtual void Fire();

	void PlayFireEffects(const FVector &TracerEndPoint);

	void PlayImpactEffects(EPhysicalSurface Surface, FVector ImpactPoint);

	UFUNCTION(Server,Reliable,WithValidation)
	virtual void ServerFire();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components" )
	USkeletalMeshComponent* SkelMeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf <UDamageType> DamageType;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName MuzzleSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* DefaultImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* FleshImpactEffect;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	UParticleSystem* SmokeBeam;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<UCameraShake> CameraShakeClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float DefaultDamage;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float FireRate;

	UPROPERTY(ReplicatedUsing = OnRep_HitScanTrace)
	FHitScanTrace HitScanTrace;

	UFUNCTION()
	void OnRep_HitScanTrace();

private:
	FTimerHandle TimerHandle;
	float LastFireTime;
	float TimeBetweenShots;

};
