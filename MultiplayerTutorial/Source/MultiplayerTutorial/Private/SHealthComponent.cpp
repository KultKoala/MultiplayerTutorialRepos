// Fill out your copyright notice in the Description page of Project Settings.

#include "SHealthComponent.h"
#include "Net/UnrealNetwork.h"
#include "SGameMode.h"
#include "Engine/World.h"


// Sets default values for this component's properties
USHealthComponent::USHealthComponent()
{
	Health = 100.0f;
	bIsDead = false;
}


// Called when the game starts
void USHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	//only hook if we are server
	if (GetOwnerRole() == ROLE_Authority) {
		AActor* Owner = GetOwner();
		if (Owner) {
			Owner->OnTakeAnyDamage.AddDynamic(this, &USHealthComponent::DamageTaken);
		}
	}
	
	DefaultHealth = Health;
}

void USHealthComponent::DamageTaken(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser) {

	if (Damage <= 0 && !bIsDead) return;

	//update health clamped
	Health = FMath::Clamp(Health - Damage, 0.0f,DefaultHealth);

	OnHealthChanged.Broadcast(this,Health,Damage,DamageType,InstigatedBy,DamageCauser);

	if (Health <= 0.0f) {
		bIsDead = true;
		ASGameMode * GM = Cast<ASGameMode>(GetWorld()->GetAuthGameMode());
		if (GM) {
			
			GM->OnActorKilled.Broadcast(DamagedActor, DamageCauser, InstigatedBy);
		}
	}

}

void USHealthComponent::OnRep_Health(float OldHealth)
{
	float Damage = OldHealth - Health;
	OnHealthChanged.Broadcast(this, Health, Damage ,nullptr,nullptr,nullptr );
}

void USHealthComponent::Heal(float HealAmount)
{
	if (Health <= 0.0f || HealAmount <= 0.0f) {
		return;
	}

	Health = FMath::Clamp(Health+ HealAmount,0.0f, DefaultHealth);

	UE_LOG(LogTemp, Log, TEXT("Health Changed: %s (+%s)"), *FString::SanitizeFloat(Health), *FString::SanitizeFloat(HealAmount))
	OnHealthChanged.Broadcast(this, Health, HealAmount, nullptr, nullptr, nullptr);
}

float USHealthComponent::GetHealth() const
{
	return Health;
}

void USHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USHealthComponent, Health);
}

