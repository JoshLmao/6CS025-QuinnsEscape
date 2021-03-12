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
	
protected:
	// Override apply effects
	virtual void ApplyEffects(APawn* pawn);
};
