// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SGameState.generated.h"

UENUM(BlueprintType)
enum class EWaveState : uint8
{
	WaitingForNextWave,

	//in the process of spawning all bots
	WaveInProgress,

	//waiting for player to kill all of the bots
	WaitingToComplete,

	WaveComplete,

	GameOver
};


/**
 * 
 */
UCLASS()
class MULTIPLAYERTUTORIAL_API ASGameState : public AGameStateBase
{
	GENERATED_BODY()

protected:
	UFUNCTION()
	void OnRep_WaveState(EWaveState OldState);

	UFUNCTION(BlueprintImplementableEvent)
	void WaveStateChanged(EWaveState OldState, EWaveState NewState);

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_WaveState, Category = "GameState")
	EWaveState WaveState;

public:
	void SetWaveState(EWaveState NewState);
};
