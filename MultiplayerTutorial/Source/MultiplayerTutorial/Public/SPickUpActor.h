// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPickUpActor.generated.h"

class USphereComponent;
class UDecalComponent;
class ASPowerUpActor;

UCLASS()
class MULTIPLAYERTUTORIAL_API ASPickUpActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPickUpActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	USphereComponent* SphereComp;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UDecalComponent* DecalComp;

	UPROPERTY(EditInstanceOnly, Category = "PickupActor")
	TSubclassOf<ASPowerUpActor> PowerUpActorClass;

	ASPowerUpActor * PowerUpActorInstance;

	void SpawnPowerUp();

	UPROPERTY(EditDefaultsOnly, Category = "PickupActor")
	float RespawnTime;

	FTimerHandle TimerHandle_RespawnTimer;

public:	

	virtual void NotifyActorBeginOverlap(AActor *OtherActor) override;


	
	
};
