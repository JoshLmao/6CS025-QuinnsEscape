// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Level0HUD.generated.h"

/*
*	HUD State enum containing all possible HUD states for a level
*/
UENUM()
enum EHUDState
{
	// Gameplay state
	Gameplay = 0,			//UMETA(DisplayName = "Gameplay")
	// The level is complete, player completed level
	LevelComplete = 1,		//UMETA(DisplayName = "LevelComplete")
	// Player has died, player failed level
	LevelFailed = 2,		//UMETA(DisplayName = "LevelFailed")
};

/**
 *	Main HUD class for displaying and setting the correct state during gameplay
 */
UCLASS()
class QUINNSESCAPE_API ALevel0HUD : public AHUD
{
	GENERATED_BODY()
	
public:
	ALevel0HUD();

	/*
	*	 VARIABLES
	*/
protected:
	// All on screen widgets to add to the HUD on BeginPlay during normal gameplay
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class UUserWidget>> GameplayUIWidgets;
	// End Level UI widgets to display
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class UUserWidget>> LevelCompleteWidgets;
	// All widgets to display when the level has failed
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class UUserWidget>> LevelFailWidgets;

private:
	// Current list of created UI widgets that are constantly active on screen
	UPROPERTY()
	TArray<class UUserWidget*> m_createdGameplayWidgets;
	// Current list of created UI widgets for level complete
	UPROPERTY()
	TArray<class UUserWidget*> m_createdLvlCompleteWidgets;
	// List of created UI widgets for level failed
	UPROPERTY()
	TArray<class UUserWidget*> m_createdLvlFailedWidgets;

	/*
	*	 METHODS
	*/
public:
	// Set the HUD state to display the correct widgets for that state
	void SetHUDDisplayState(EHUDState hudState);

protected:
	virtual void BeginPlay() override;

private:
	// Creates widgets from list of widget classes and returns the list of created widgets.
	// Can be added to viewport if addToViewport is true
	TArray<class UUserWidget*> CreateAllWidgets(TArray<TSubclassOf<class UUserWidget>> widgets, bool addToViewport = false);
	
	// Sets a list of created widgets to be added or removed from viewport, depending on addToViewport parameter
	void SetWidgetsToViewport(TArray<class UUserWidget*> instWidgets, bool addToViewport);
};
