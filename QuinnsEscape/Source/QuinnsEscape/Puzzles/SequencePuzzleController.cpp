// Fill out your copyright notice in the Description page of Project Settings.


#include "SequencePuzzleController.h"
#include "Puzzles/PuzzleEscapeDoor.h"
#include "Puzzles/PuzzleSequenceBox.h"
#include "Engine/World.h"
#include "Game/QuinnGameState.h"

// Sets default values
ASequencePuzzleController::ASequencePuzzleController()
{
	ScoreReward = 100;

}

// Called when the game starts or when spawned
void ASequencePuzzleController::BeginPlay()
{
	Super::BeginPlay();
	
	// Iterate through all sequence boxes
	for (AActor* box : SequenceBoxes)
	{
		// Cast to sequence box class
		APuzzleSequenceBox* sequenceBox = Cast<APuzzleSequenceBox>(box);
		if (IsValid(sequenceBox))
		{
			// Listen to it's triggered event
			sequenceBox->OnSequenceBoxTriggered.AddDynamic(this, &ASequencePuzzleController::OnSequenceBoxTriggered);
		}
	}
}

void ASequencePuzzleController::OnSequenceBoxTriggered(AActor* triggeredBox, int triggeredBoxIndex)
{
	if (m_isPuzzleSolved)
	{
		// Dont allow editing of sequence once complete
		return; 
	}

	int attemptingIndex = m_currentSequence.Num();

	bool isSuccessful = false;
	// Check index is within bounds of expected sequence
	if (attemptingIndex >= 0 && attemptingIndex < CorrectSequence.Num())
	{
		// If current triggered box's index equals expected index in sequence at this point
		if (triggeredBoxIndex == CorrectSequence[attemptingIndex])
		{
			// Add to current sequence
			m_currentSequence.Add(triggeredBoxIndex);

			isSuccessful = true;
		}
	}

	// If player found the correct box this turn
	if (isSuccessful)
	{
		UE_LOG(LogTemp, Log, TEXT("Puzzle 2: Successfully found matching index '%d' at index '%d' in sequence"), triggeredBoxIndex, attemptingIndex);

		// Set triggered box text to green for hint
		Cast<APuzzleSequenceBox>(triggeredBox)->SetIndexTextColor(FColor::Green);

		// Validate if sequence if fully complete, complete if so
		if (CorrectSequence.Num() == m_currentSequence.Num())
		{
			OnSequenceComplete();
			UE_LOG(LogTemp, Log, TEXT("Sequence puzzle complete!"));
		}
	}
	else
	{
		// player wasn't successful. remove success colors and clear
		SetAllTextColor(SequenceBoxes, FColor::Black);
		
		// Empty sequence arrays
		m_currentSequence.Empty();

		UE_LOG(LogTemp, Log, TEXT("Wrong box index '%d'! Cleared current sequence"), triggeredBoxIndex);
	}
}

void ASequencePuzzleController::OnSequenceComplete()
{
	m_isPuzzleSolved = true;

	// Get state and add score reward for completion
	AGameStateBase* baseState = GetWorld()->GetGameState();
	if (AQuinnGameState* quinnState = Cast<AQuinnGameState>(baseState))
	{
		quinnState->AddScore(ScoreReward);
	}

	// Check valid and open the escape
	if (IsValid(EscapeDoor))
	{
		// Open exit door
		EscapeDoor->OpenEscape();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No escape door set! Unable to open escape door"));
	}
}

void ASequencePuzzleController::SetAllTextColor(TArray<AActor*> allBoxes, FColor color)
{
	// Iterate through each box and set color
	for (AActor* actorBox : allBoxes)
	{
		APuzzleSequenceBox* box = Cast<APuzzleSequenceBox>(actorBox);
		box->SetIndexTextColor(color);
	}
}
