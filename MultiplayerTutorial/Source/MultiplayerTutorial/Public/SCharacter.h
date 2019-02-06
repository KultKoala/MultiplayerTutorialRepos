// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class ASWeapon;
class USHealthComponent;

UCLASS()
class MULTIPLAYERTUTORIAL_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);

	void BeginCrouch();
	void EndCrouch();

	void BeginZoom();
	void EndZoom();

	void StartFire();
	void EndFire();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent * CameraComp;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Components")
	USpringArmComponent *SpringComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USHealthComponent *HealthComp;

	bool bWantsToZoom;
	
	UPROPERTY(EditDefaultsOnly, Category = "Camera", meta = (ClampMin = 0.1,ClampMax = 100.0))
	float ZoomSpeed;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float ZoomedFOV;

	float DefaultFOV;

	UPROPERTY(Replicated)
	ASWeapon * CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<ASWeapon> WeaponClass;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
	FName WeaponSocket;

	UFUNCTION()
	void OnHealthChanged(USHealthComponent* HComp, float Health, float HealthDelta, const UDamageType * DamageType,
						AController * InstigatedBy, AActor * DamageCauser);

	UPROPERTY(Replicated,BlueprintReadOnly, Category = "Player")
	bool bDied;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override;
	
};
