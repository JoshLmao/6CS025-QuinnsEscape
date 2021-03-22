// Fill out your copyright notice in the Description page of Project Settings.


#include "QEPlayerSaveData.h"

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
