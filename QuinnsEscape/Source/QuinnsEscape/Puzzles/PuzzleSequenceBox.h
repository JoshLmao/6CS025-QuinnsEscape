// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "World/HeadHittableBox.h"
#include "PuzzleSequenceBox.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSequenceBoxOnTriggeredSignature, AActor*, triggeredBox, int, seqenceBoxIndex);

/**
 *	A single sequence box that allows to be "head hit" to trigger
 */
UCLASS()
class QUINNSESCAPE_API APuzzleSequenceBox : public AHeadHittableBox
{
	GENERATED_BODY()

public:
	// Default constructor
	APuzzleSequenceBox();

	/*
	*	VARIABLES
	*/
public:
	// Index
	UPROPERTY(EditAnywhere)
	int SequenceIndex;

protected:
	// Text beside box to display its index
	class ATextRenderActor* IndexText;

	/*
	*	EVENTS
	*/
public:
	// Event triggered when this box is triggered
	UPROPERTY(BlueprintAssignable)
	FSequenceBoxOnTriggeredSignature OnSequenceBoxTriggered;

	/*
	*	METHODS
	*/
public:
	virtual void BeginPlay() override;

	// Set the color of the IndexText variable
	void SetIndexTextColor(FColor color);

private:
	UFUNCTION()
	void OnBoxRecievedHeadHit(ACharacter* character);
};
