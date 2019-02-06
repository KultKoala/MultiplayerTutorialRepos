// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SPowerUpActor.generated.h"

UCLASS()
class MULTIPLAYERTUTORIAL_API ASPowerUpActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASPowerUpActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//keeps state of the power up for replication
	UPROPERTY(ReplicatedUsing=OnRep_PowerUpActive)
	bool bIsPowerUpActive;

	UFUNCTION()
	void OnRep_PowerUpActive();

	UFUNCTION(BlueprintImplementableEvent, Category = "PowerUps")
	void OnPowerUpStateChanged(bool NewIsActive);

	UPROPERTY(EditDefaultsOnly, Category = "PowerUps")
	float PowerUpInterval;

	/*Total number of times we apply the power up event*/
	UPROPERTY(EditDefaultsOnly, Category = "PowerUps")
	int32 TotalNumberofTicks;

	int32 TicksProcessed;

	FTimerHandle TimerHandle_PowerUpTicks;

	UFUNCTION()
	void OnTickPowerUp();

public:	

	void ActivatePowerUp();

	UFUNCTION(BlueprintImplementableEvent, Category = "PowerUps")
	void OnActivated();

	UFUNCTION(BlueprintImplementableEvent, Category = "PowerUps")
	void OnPowerUpTicked();

	UFUNCTION(BlueprintImplementableEvent, Category = "PowerUps")
	void OnExpired();
	
};
