// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "STrackerBot.generated.h"


class UStaticMeshComponent;
class USHealthComponent;
class UMaterialInstanceDynamic;
class UParticleSystem;
class USphereComponent;
class USoundCue;

UCLASS()
class MULTIPLAYERTUTORIAL_API ASTrackerBot : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASTrackerBot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHealthChanged(USHealthComponent * HComp, float Health, float HealthDelta, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser);

	UPROPERTY(VisibleDefaultsOnly, Category = "Components")
	UStaticMeshComponent *MeshComp;

	FVector GetNextPathPoint();

	void MoveTowardsNextPoint();

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float MovementForce;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	bool bUseVelocityChange;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float RequiredDistanceToTarget;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float BaseDamage;

	UPROPERTY(EditDefaultsOnly, Category = "TrackerBot")
	float DamageRadius;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	USHealthComponent * HealthComp;

	//Dynamic Material to pulse on damage
	UPROPERTY(BlueprintReadOnly, Category = "Components")
	UMaterialInstanceDynamic *DynamicMat;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	USphereComponent *SphereComp;

	void HurtSelf();

	void SelfDestruct();

	FTimerHandle TimerHandle_DamageSelf;

	//particle effect to be play when the bot explodes
	UPROPERTY(EditDefaultsOnly, Category = "Visual Effects")
	UParticleSystem* BotExplosion;

	FVector NextPoint;
	bool bDied;

	UPROPERTY(EditDefaultsOnly, Category = "Sound Effects")
	USoundCue * SelfDestructSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound Effects")
	USoundCue * ExplosionSound;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor *OtherActor) override;
	
};
