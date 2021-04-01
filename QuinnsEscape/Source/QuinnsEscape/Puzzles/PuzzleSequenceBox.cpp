// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleSequenceBox.h"
#include "GameFramework/Character.h"
#include "Characters/QuinnCharacter.h"

APuzzleSequenceBox::APuzzleSequenceBox()
{
	SequenceIndex = 0;

	// Listen to recieved hit event
	this->OnRecievedHeadHit.AddDynamic(this, &APuzzleSequenceBox::OnBoxRecievedHeadHit);
}

void APuzzleSequenceBox::BeginPlay()
{
	Super::BeginPlay();

	// Add sequence index to side of box
	IndexText = CreateBoxWithText(FString::FromInt(SequenceIndex));
}

void APuzzleSequenceBox::SetIndexTextColor(FColor color)
{
	// if indextext is set, set the color
	if (IndexText)
	{
		SetTextColor(IndexText, color);
	}
}

void APuzzleSequenceBox::OnBoxRecievedHeadHit(ACharacter* character)
{
	// Check character is main character
	if (character->IsA(AQuinnCharacter::StaticClass()))
	{
		// Check event is bound
		if (OnSequenceBoxTriggered.IsBound())
		{
			// Broadcast with it's index
			OnSequenceBoxTriggered.Broadcast(this, SequenceIndex);
		}
	}
}