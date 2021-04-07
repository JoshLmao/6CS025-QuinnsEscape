// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "QEPlayerSaveData.generated.h"

/*
*	Data tracked for a single game of Quinn's Escape. Struct is stored in save game file.
*	Only track vital/minimal information of a game
*/
USTRUCT(BlueprintType, Blueprintable)
struct FQESingleGameData
{
	GENERATED_BODY()

	// Time in seconds to complete the level
	UPROPERTY(BlueprintReadWrite)
	float CompleteSeconds;
	// Amount of score the player achieved
	UPROPERTY(BlueprintReadWrite)
	float Score;
	// ISO8601 string representation of the date time this game was completed
	UPROPERTY(BlueprintReadWrite)
	FString CompleteDateTime;

	// Default constructor
	FORCEINLINE FQESingleGameData();
	// Constructor with all properties
	explicit FORCEINLINE FQESingleGameData(float time, float score, FString isoDateTimeStr = "");
};

FORCEINLINE FQESingleGameData::FQESingleGameData()
{
	CompleteDateTime = FDateTime::UtcNow().ToIso8601();
	CompleteSeconds = 0.0f;
	Score = 0.0f;
}

FORCEINLINE FQESingleGameData::FQESingleGameData(const float time, const float score, FString isoDateTimeStr)
{
	CompleteSeconds = time;
	Score = score;

	// If no iso string, set datetime to now
	if (isoDateTimeStr.IsEmpty())
		CompleteDateTime = FDateTime::UtcNow().ToIso8601();
	else
		CompleteDateTime = isoDateTimeStr;
}

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
	// Save slot name to use
	static const FString SAVE_GAME_SLOT;
	// Save slot player index to use
	static const int SAVE_GAME_INDEX = 1;

	// Maximum amount of games to store in game history
	const int MAX_GAME_HISTORY = 25;
	// Maximum amount of games to be stores as a high score
	const int MAX_HIGH_SCORE_GAMES = 5;

	// List of all games played by player
	UPROPERTY(VisibleAnywhere, Category = Basic)
	TArray<FQESingleGameData> GameHistory;

	// List of all high scores
	UPROPERTY(VisibleAnywhere, Category = Basic)
	TArray<FQESingleGameData> HighScores;

	/*
	*	METHODS
	*/
public:
	// Adds a game to the game history, also adds to high scores if necessary
	void AddGameToHistory(FQESingleGameData game);
	// Checks the given game to determine if it's a new high score against previous scores and adds it to the HighScores array if true
	// Returns a bool of if the game was added or not
	bool ValidateHighScore(FQESingleGameData game);

	// Get all high scores, stored in order of highest. 0 = highest ever score
	TArray<FQESingleGameData> GetHighScores();
	// Get all games played
	TArray<FQESingleGameData> GetGameHistory();

	// Gets the current saved data
	UFUNCTION(BlueprintCallable)
	static UQEPlayerSaveData* GetCurrentSaveData();
	// Saves the given save data
	UFUNCTION(BlueprintCallable)
	static bool SaveData(UQEPlayerSaveData* saveData);
	// Deletes all save data
	UFUNCTION(BlueprintCallable)
	static bool DeleteAllData();
};
