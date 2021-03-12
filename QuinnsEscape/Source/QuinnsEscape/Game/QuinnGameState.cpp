// Fill out your copyright notice in the Description page of Project Settings.


#include "QuinnGameState.h"
#include "TimerManager.h"

int AQuinnGameState::GetScore()
{
	return m_totalScore;
}

void AQuinnGameState::AddScore(int amount)
{
	// Add amount of score to total
	m_totalScore += (amount * m_currentScoreMultiplier);
}

void AQuinnGameState::SetDurationScoreMultiplier(float multiplier, float durationSeconds)
{
	// Check if multiplier is already in progress
	if (GetWorldTimerManager().IsTimerPending(m_durationScoreMultiHandle))
	{
		// If in progress, clear previous score 
		GetWorldTimerManager().ClearTimer(m_durationScoreMultiHandle);
		UE_LOG(LogTemp, Log, TEXT("Clearing previous score multiplier"));
	}

	// Set new multiplier
	m_currentScoreMultiplier = multiplier;
	// Start timer to end multiplier after seconds
	GetWorldTimerManager().SetTimer(m_durationScoreMultiHandle, this, &AQuinnGameState::OnDurationScoreMultiplierEnded, durationSeconds, false);

	UE_LOG(LogTemp, Log, TEXT("Set score multiplier to '%f', for '%f' seconds"), m_currentScoreMultiplier, durationSeconds);
}

void AQuinnGameState::OnDurationScoreMultiplierEnded()
{
	// Set multiplier back to 1
	m_currentScoreMultiplier = 1;

	UE_LOG(LogTemp, Log, TEXT("Set score multiplier back to '%f'"), m_currentScoreMultiplier);
}
