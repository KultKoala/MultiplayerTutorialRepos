// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnActorKilled, AActor*, VictimActor, AActor*, KillerActor, AController*, KillerController);

enum class EWaveState: uint8;

/**
 * 
 */
UCLASS()
class MULTIPLAYERTUTORIAL_API ASGameMode : public AGameModeBase
{
	GENERATED_BODY()


protected:

	UFUNCTION(BlueprintImplementableEvent, Category = "Events")
	void SpawnANewBot();

	void SpawnBotTimerElapsed();

	//Start spawning bots
	void StartWave();

	//stop spawning bots
	void EndWave();
	
	//set timer for next wave
	void PrepareForNextWave();

	void CheckWaveState();

	void CheckForAlivePlayers();

	void GameOver();

	void SetWaveState(EWaveState);

	void RestartDeadPlayers();
	
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
		int32 NumOfBotsToSpawn;

	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
		int32 WaveCount;

	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
		float SpawnRate;

	UPROPERTY(EditDefaultsOnly, Category = "GameMode")
		float TimeBetweenWaves;

	FTimerHandle TimerHandle_WaveTimer;

	FTimerHandle TimerHandle_BotTimer;

public:
	ASGameMode();

	virtual void Tick(float DeltaSeconds) override;

	virtual void StartPlay() override;

	UPROPERTY(BlueprintAssignable, Category = "GameMode")
	FOnActorKilled OnActorKilled;
	
};
