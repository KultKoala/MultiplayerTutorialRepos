// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SExplosiveBarrel.generated.h"

class UStaticMeshComponent;
class UParticleSystem;
class UMaterialInterface;
class USHealthComponent;

UCLASS()
class MULTIPLAYERTUTORIAL_API ASExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASExplosiveBarrel();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent *MeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USHealthComponent *HealthComp;

	UPROPERTY(EditDefaultsOnly, Category = "Particle Effects")
	UParticleSystem * ExplosionEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Materials")
	UMaterialInterface * IntactMaterial;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Materials")
	UMaterialInterface * ExplodedMaterial;

	bool bExploded;

	UFUNCTION()
	void OnHealthChanged(USHealthComponent * HComp, float Health, float HealthDelta, const UDamageType * DamageType, AController * InstigatedBy, AActor * DamageCauser);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Explode();

	

	
	
};
