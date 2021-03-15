// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PowerUps/PowerupPickup.h"
#include "ExtraLifePowerup.generated.h"

/**
 * 
 */
UCLASS()
class QUINNSESCAPE_API AExtraLifePowerup : public APowerupPickup
{
	GENERATED_BODY()
	
public:
	AExtraLifePowerup();

	/*
	*	VARIABLES
	*/
public:
	// Amount of extra lives to reward to the player
	UPROPERTY(EditAnywhere)
	int ExtraLivesAmount;

	// Amount of score to give to player if their lives are full
	UPROPERTY(EditAnywhere)
	int BonusScore;

	/*
	*	METHODS
	*/
protected:
	// Override to apply effects
	virtual void ApplyEffects(APawn* collidedPawn) override;
};
