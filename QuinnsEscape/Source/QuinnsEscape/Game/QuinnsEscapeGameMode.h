// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "QuinnsEscapeGameMode.generated.h"

/*
*	Main game mode for Quinns Escape
*/
UCLASS(minimalapi)
class AQuinnsEscapeGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	// Default constructor
	AQuinnsEscapeGameMode();

	// Save slot name to use
	const FString SAVE_GAME_SLOT = "SLOT_1";
	// Save slot player index to use
	const int SAVE_GAME_INDEX = 1;

private:
	bool m_isLevelOver;

	/*
	*	METHODS
	*/
protected:
	// Start/Begin play
	virtual void StartPlay() override;

	// Called when game has finished, either by complete to failure
	virtual void OnGameOver(bool didCompleteLevel);

private:
	// Reciever of event from EndLevelTrigger
	UFUNCTION()
	void OnLevelComplete(AActor* endingTrigger, ACharacter* quinnChar);

	// Reciever of event from Quinn OnCharacterDied
	UFUNCTION()
	void OnQuinnDeath();

	// Binds any events that should trigger a "level complete"
	bool TryBindLevelCompleteEvent();
	// Bind to any events that should trigger a "level failed"
	bool TryBindLevelFailedEvent();

	// Adds the given game data to the current save file
	void AddGameToSaveGame(struct FQESingleGameData gameData);
};
