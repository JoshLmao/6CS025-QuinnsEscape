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

void ACoinBonusPowerup::ApplyEffects(APawn* pawn)
{
	Super::ApplyEffects(pawn);

	// Cast to quinn
	AGameStateBase* baseState = GetWorld()->GetGameState();
	if (AQuinnGameState* gameState = Cast<AQuinnGameState>(baseState))
	{
		// Add score before multiplier
		gameState->AddScore(InitialScore);
		// Set score multiplier
		gameState->SetDurationScoreMultiplier(ScoreMultiplier, MultiplierDuration);
	}
}