// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreWidget.h"
#include "../Game/QuinnGameState.h"

void UScoreWidget::NativeConstruct()
{
	Super::NativeConstruct();

	AGameStateBase* state = GetWorld()->GetGameState();
	if (state != nullptr)
	{
		QuinnGameState = Cast<AQuinnGameState>(state);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to determine QuinnGameState. UI Widget won't work!"));
	}
}

AQuinnGameState* UScoreWidget::GetQuinnGameState()
{
	return QuinnGameState;
}

float UScoreWidget::GetCurrentScore()
{
	if (IsValid(QuinnGameState))
	{
		return QuinnGameState->GetScore();
	}
	return 0;
}

float UScoreWidget::GetCurrentScoreMultiplier()
{
	if (IsValid(QuinnGameState))
	{
		return QuinnGameState->GetScoreMultiplier();
	}
	return 1.0f;
}