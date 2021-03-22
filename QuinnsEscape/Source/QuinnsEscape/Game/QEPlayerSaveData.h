// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "QEPlayerSaveData.generated.h"

/*
*	Data tracked for a single game of Quinn's Escape
*/
USTRUCT()
struct FQESingleGameData
{
	GENERATED_BODY()

	// Time in seconds to complete the level
	UPROPERTY(VisibleAnywhere, Category = Basic)
	float TimeSeconds;
	// Amount of score the player achieved
	UPROPERTY(VisibleAnywhere, Category = Basic)
	double Score;
	// ISO8601 string representation of the date time this game was added
	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString DateTime;

	// Default constructor
	FQESingleGameData() 
	{
		DateTime = GetIsoDateTimeNow();
	}

	// Constructor with all properties
	FQESingleGameData(float time, double score)
	{
		TimeSeconds = time;
		Score = score;
		DateTime = GetIsoDateTimeNow();
	}

	// Gets the ISO8601 string representation of the time now
	FString GetIsoDateTimeNow() { return FDateTime::UtcNow().ToIso8601(); }
};

/**
 *	Save game class for handling any data that is required over sessions.
 */
UCLASS()
class QUINNSESCAPE_API UQEPlayerSaveData : public USaveGame
{
	GENERATED_BODY()
	
public:
	UQEPlayerSaveData();

	/*
	*	VARIABLES
	*/
public:
	// Maximum amount of games to store in game history
	const int MAX_GAME_HISTORY = 25;

	// List of all games played by player
	UPROPERTY(VisibleAnywhere, Category = Basic)
	TArray<FQESingleGameData> GameHistory;

	/*
	*	METHODS
	*/
public:
	// Adds a game to the game history, also adds to high scores if necessary
	void AddGameToHistory(FQESingleGameData game);

};
