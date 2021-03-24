// Fill out your copyright notice in the Description page of Project Settings.


#include "QEPlayerSaveData.h"
#include "Kismet/GameplayStatics.h"

// Set const FString
const FString UQEPlayerSaveData::SAVE_GAME_SLOT = FString::Printf(TEXT("SLOT_1"));

UQEPlayerSaveData::UQEPlayerSaveData()
{
}

void UQEPlayerSaveData::AddGameToHistory(FQESingleGameData game)
{
	// Check if history contains >= maximum
	if (GameHistory.Num() >= MAX_GAME_HISTORY)
	{
		// Remove oldest game in history
		GameHistory.RemoveAt(0);
	}

	// Append current game to list
	GameHistory.Add(game);
}

bool UQEPlayerSaveData::ValidateHighScore(FQESingleGameData game)
{
	if (HighScores.Num() <= 0)
	{
		// No high scores, add game
		HighScores.Add(game);
		return true;
	}
	else // HighScores.Num() > 0
	{
		// Iterate though all high scores to see if current game beats a previous
		for (int i = 0; i < HighScores.Num(); i++)
		{
			// Check new game score is more than the current iterations game's score
			if (game.Score > HighScores[i].Score)
			{
				// Insert at index above, place higher than current iteration
				HighScores.Insert(game, i);

				// Trim last game from array if array is now bigger than MAX
				if (HighScores.Num() > MAX_HIGH_SCORE_GAMES) {
					HighScores.RemoveAt(MAX_HIGH_SCORE_GAMES);
				}
				return true;
			}
			// If new game score equals an existing score and index isn't the last score in list
			else if (game.Score == HighScores[i].Score && i != (MAX_HIGH_SCORE_GAMES - 1))
			{
				// Insert at index below, prefer to use previous score as higher ranking
				HighScores.Insert(game, i + 1);

				// Trim last game from array if array is now bigger than MAX
				if (HighScores.Num() > MAX_HIGH_SCORE_GAMES) {
					HighScores.RemoveAt(MAX_HIGH_SCORE_GAMES);
				}
				return true;
			}
		}
	}

	return false;
}

TArray<FQESingleGameData> UQEPlayerSaveData::GetHighScores()
{
	return HighScores;
}

TArray<FQESingleGameData> UQEPlayerSaveData::GetGameHistory()
{
	return GameHistory;
}

UQEPlayerSaveData* UQEPlayerSaveData::GetCurrentSaveData()
{
	if (UGameplayStatics::DoesSaveGameExist(SAVE_GAME_SLOT, SAVE_GAME_INDEX))
	{
		// Load existing save game
		UQEPlayerSaveData* saveGame = Cast<UQEPlayerSaveData>(UGameplayStatics::LoadGameFromSlot(SAVE_GAME_SLOT, SAVE_GAME_INDEX));
		return saveGame;
	}
	return nullptr;
}

bool UQEPlayerSaveData::SaveData(UQEPlayerSaveData* saveData)
{
	return UGameplayStatics::SaveGameToSlot(saveData, SAVE_GAME_SLOT, SAVE_GAME_INDEX);
}

bool UQEPlayerSaveData::DeleteAllData()
{
	// Debug: delete for looping between these two
	return UGameplayStatics::DeleteGameInSlot(SAVE_GAME_SLOT, SAVE_GAME_INDEX);
}