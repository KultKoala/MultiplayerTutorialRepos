// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SGrenade.generated.h"

class USphereComponent;
class UProjectileMovementComponent;


UCLASS()
class MULTIPLAYERTUTORIAL_API ASGrenade : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASGrenade();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent * MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent * ExplosionMeshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Particle Effects")
	UParticleSystem * ExplosionEffect;

	UPROPERTY(EditAnywhere, Category = "Components")
	UProjectileMovementComponent * ProjectileComp;

	UPROPERTY(EditAnywhere, Category = "Damage")
	float Damage;

	FTimerHandle TimerHandle;

	void Explode();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
