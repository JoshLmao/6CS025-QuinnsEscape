// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "QuinnGameState.generated.h"

/**
 * Main game state that handles the overall level/game information
 */
UCLASS()
class QUINNSESCAPE_API AQuinnGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AQuinnGameState();

	/*
	*	VARIABLES
	*/
private:
	// Total score of this play session
	double m_totalScore;
	// Current multiplier to apply when adding any new score
	float m_currentScoreMultiplier;

	// Timer handle that tracks a duration score multiplier
	FTimerHandle m_durationScoreMultiHandle;

	/*
	*	METHODS
	*/
public:
	// Gets the current score
	double GetScore();
	// Adds an amount to the current score
	void AddScore(double amount);

	// Gets the current score multiplier
	float GetScoreMultiplier();
	// Sets a duration multiplier for the score for a duration
	void SetDurationScoreMultiplier(float multiplier, float durationSeconds);

private:
	// Called when duration score multiplier is over
	void OnDurationScoreMultiplierEnded();
};
