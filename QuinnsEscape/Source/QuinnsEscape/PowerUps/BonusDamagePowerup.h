// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerUps/PowerupPickup.h"
#include "BonusDamagePowerup.generated.h"

/**
 * 
 */
UCLASS()
class QUINNSESCAPE_API ABonusDamagePowerup : public APowerupPickup
{
	GENERATED_BODY()
	
public:
	ABonusDamagePowerup();

	/*
	*	VARIABLES
	*/
public:
	// Duration in seconds to apply bonus damage to character
	UPROPERTY(EditAnywhere)
	int Duration;
	// Multiplier value to use on regular damage when character deals damage to another character
	UPROPERTY(EditAnywhere)
	float DamageMultiplier;

	/*
	*	METHODS
	*/
public:
	virtual void ApplyEffects(APawn* collidedPawn) override;
};
