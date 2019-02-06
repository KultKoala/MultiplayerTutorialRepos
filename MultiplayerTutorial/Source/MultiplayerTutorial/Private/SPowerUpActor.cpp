// Fill out your copyright notice in the Description page of Project Settings.

#include "SPowerUpActor.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"


// Sets default values
ASPowerUpActor::ASPowerUpActor()
{
	PowerUpInterval = 0.0f;
	TotalNumberofTicks = 0;
	TicksProcessed = 0;
	bIsPowerUpActive = false;

}

// Called when the game starts or when spawned
void ASPowerUpActor::BeginPlay()
{
	Super::BeginPlay();

}

void ASPowerUpActor::OnRep_PowerUpActive()
{
	OnPowerUpStateChanged(bIsPowerUpActive);
}

void ASPowerUpActor::OnTickPowerUp()
{
	TicksProcessed++;
	OnPowerUpTicked();

	if (TicksProcessed >= TotalNumberofTicks) {
		OnExpired();
		bIsPowerUpActive = false;
		OnRep_PowerUpActive();
		GetWorldTimerManager().ClearTimer(TimerHandle_PowerUpTicks);
	}

}

void ASPowerUpActor::ActivatePowerUp()
{
	OnActivated();
	bIsPowerUpActive = true;
	OnRep_PowerUpActive();

	if (PowerUpInterval > 0) {
		GetWorldTimerManager().SetTimer(TimerHandle_PowerUpTicks, this, &ASPowerUpActor::OnTickPowerUp, PowerUpInterval, true);
	}
	else {
		OnTickPowerUp();
	}

}


void ASPowerUpActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPowerUpActor, bIsPowerUpActive);
}
