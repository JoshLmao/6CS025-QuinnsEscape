// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerUps/PowerupPickup.h"
#include "CoinBonusPowerup.generated.h"

/**
 *	Coin bonus powerup that rewards the player with an instant coin bonus, as well as
 *	a coin multiplier for 30 seconds.
 */
UCLASS()
class QUINNSESCAPE_API ACoinBonusPowerup : public APowerupPickup
{
	GENERATED_BODY()

public:
	// Default constructor
	ACoinBonusPowerup();

	/*
	*	VARIABLES
	*/
public:
	// Amount of score to initially give after picking up
	UPROPERTY(EditAnywhere)
	int InitialScore;
	// Amount of multiplier to give after picking up
	UPROPERTY(EditAnywhere)
	float ScoreMultiplier;
	// Duration in seconds to apply the multiplier
	UPROPERTY(EditAnywhere)
	float MultiplierDuration;

	/*
	*	METHODS
	*/
protected:
	// Override apply effects
	virtual void ApplyEffects(APawn* collidedPawn) override;
};
