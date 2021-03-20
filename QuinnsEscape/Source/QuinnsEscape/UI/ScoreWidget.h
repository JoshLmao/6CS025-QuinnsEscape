// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreWidget.generated.h"

/**
 * 
 */
UCLASS()
class QUINNSESCAPE_API UScoreWidget : public UUserWidget
{
	GENERATED_BODY()
	
	/*
	*	VARIABLES
	*/
private:
	class AQuinnGameState* QuinnGameState;

	/*
	*	METHODS
	*/
protected:
	// Called when Widget gets constructed
	virtual void NativeConstruct() override;

	// Gets the current game state in the world, cast to AQuinnGameState
	UFUNCTION(BlueprintCallable)
	class AQuinnGameState* GetQuinnGameState();

	// Gets the current score
	UFUNCTION(BlueprintCallable)
	float GetCurrentScore();
	// Gets the current multiplier
	UFUNCTION(BlueprintCallable)
	float GetCurrentScoreMultiplier();

};
