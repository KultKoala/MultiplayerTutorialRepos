// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SWeapon.h"
#include "SGrenadeLauncher.generated.h"


/**
 * 
 */
UCLASS()
class MULTIPLAYERTUTORIAL_API ASGrenadeLauncher : public ASWeapon
{
	GENERATED_BODY()
	
	virtual void Fire() override;

	UPROPERTY(EditAnywhere, Category = "ProjectileClass")
	UClass * ProjectileClass;


};
