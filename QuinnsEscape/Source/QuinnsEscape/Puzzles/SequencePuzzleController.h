// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SequencePuzzleController.generated.h"

UCLASS()
class QUINNSESCAPE_API ASequencePuzzleController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASequencePuzzleController();

	/*
	*	VARIABLES
	*/
public:
	// Array of all sequence boxes
	UPROPERTY(EditAnywhere)
	TArray<AActor*> SequenceBoxes;

	// Correct sequence of indexes required to complete this puzzle
	UPROPERTY(EditAnywhere)
	TArray<int> CorrectSequence;

	// Door to the exit of the puzzle
	UPROPERTY(EditAnywhere)
	class APuzzleEscapeDoor* EscapeDoor;

	// Amount of score to reward the player for completing the puzzle
	UPROPERTY()
	int ScoreReward;
	
private:
	// Current box sequence the player has triggered
	UPROPERTY()
	TArray<int> m_currentSequence;
	// has the puzzle been solved yet?
	bool m_isPuzzleSolved;

	/*
	*	METHODS
	*/
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnSequenceBoxTriggered(AActor* triggeredBox, int triggeredBoxIndex);

	void OnSequenceComplete();

	void SetAllTextColor(TArray<AActor*> allBoxes, FColor color);
};
