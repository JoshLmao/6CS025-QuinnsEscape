// Fill out your copyright notice in the Description page of Project Settings.


#include "CoinBonusPowerup.h"
#include "Engine/World.h"
#include "../Characters/QuinnCharacter.h"
#include "../Game/QuinnGameState.h"

ACoinBonusPowerup::ACoinBonusPowerup()
{
	// Set default values
	InitialScore = 10;
	ScoreMultiplier = 1.15;
	MultiplierDuration = 20;
}

void ACoinBonusPowerup::ApplyEffects(APawn* collidedPawn)
{
	Super::ApplyEffects(collidedPawn);

	// Get QuinnGameState
	if (AQuinnGameState* gameState = this->GetQuinnGameState())
	{
		// Add score before multiplier
		gameState->AddScore(InitialScore);
		// Set score multiplier
		gameState->SetDurationScoreMultiplier(ScoreMultiplier, MultiplierDuration);
	}
}