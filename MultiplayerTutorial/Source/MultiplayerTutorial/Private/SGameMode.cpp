// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameMode.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "SHealthComponent.h"
#include "SGameState.h"
#include "SPlayerState.h"
#include "SGameMode.h"

ASGameMode::ASGameMode()
{
	TimeBetweenWaves = 2.0f;
	WaveCount = 0;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1.0f;

	GameStateClass = ASGameState::StaticClass();
	PlayerStateClass = ASPlayerState::StaticClass();
}

void ASGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	CheckWaveState();
	CheckForAlivePlayers();
}

void ASGameMode::StartPlay()
{
	Super::StartPlay();
	PrepareForNextWave();
}

void ASGameMode::GameOver()
{
	EndWave();
	SetWaveState(EWaveState::GameOver);
	UE_LOG(LogTemp, Log, TEXT("GAME OVER!!!"));
}

void ASGameMode::SetWaveState(EWaveState NewState)
{
	ASGameState *GS = GetGameState<ASGameState>();
	if (ensureAlways(GS)) {
		GS->SetWaveState(NewState);
	}
}

void ASGameMode::RestartDeadPlayers()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It) {
		APlayerController* PC = It->Get();

		if (PC && PC->GetPawn() ==nullptr) {
			RestartPlayer(PC);
		}
	}
}

void ASGameMode::SpawnBotTimerElapsed()
{
	if (NumOfBotsToSpawn > 0) {
		SpawnANewBot();
		NumOfBotsToSpawn--;
	}
}

void ASGameMode::StartWave()
{
	WaveCount++;
	UE_LOG(LogTemp,Log, TEXT("Wave %d"), WaveCount)
	NumOfBotsToSpawn = WaveCount * 2;
	GetWorldTimerManager().SetTimer(TimerHandle_BotTimer, this, &ASGameMode::SpawnBotTimerElapsed, 1.0f,true, 0.0f);
	SetWaveState(EWaveState::WaveInProgress);
}

void ASGameMode::EndWave()
{
	GetWorldTimerManager().ClearTimer(TimerHandle_BotTimer);
	SetWaveState(EWaveState::WaveComplete);
	PrepareForNextWave();
}

void ASGameMode::PrepareForNextWave()
{
	GetWorldTimerManager().SetTimer(TimerHandle_WaveTimer, this, &ASGameMode::StartWave,TimeBetweenWaves,false);
	SetWaveState(EWaveState::WaitingForNextWave);
}

void ASGameMode::CheckWaveState()
{
	bool bIsWavePreparing = GetWorldTimerManager().IsTimerActive(TimerHandle_WaveTimer);

	if (NumOfBotsToSpawn > 0) {
		SetWaveState(EWaveState::WaitingToComplete);
		return;
	}

	if (bIsWavePreparing) {
		return;
	}

	bool bIsEnemiesLeft = false;

	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It) {
		APawn* TestPawn = It->Get();

		if (TestPawn == nullptr || TestPawn->IsPlayerControlled()) {
			continue;
		}
		
		USHealthComponent * HealthComp = Cast<USHealthComponent>(TestPawn->GetComponentByClass(USHealthComponent::StaticClass()));
		if (HealthComp && HealthComp->GetHealth() > 0.0f) {
			bIsEnemiesLeft = true;
		}
	}

	if (!bIsEnemiesLeft) {
		EndWave();
	}
}

void ASGameMode::CheckForAlivePlayers()
{
	bool bIsPlayersLeft = false;

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It) {
		APlayerController* PC = It->Get();

		if (PC && PC->GetPawn()) {

			USHealthComponent * HealthComp = Cast<USHealthComponent>(PC->GetPawn()->GetComponentByClass(USHealthComponent::StaticClass()));
			if (ensure(HealthComp) && HealthComp->GetHealth() > 0.0f) {
				bIsPlayersLeft = true;
			}
		}
	}

	if (!bIsPlayersLeft) {
		GameOver();
	}
}




