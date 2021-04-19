// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "QuinnsEscapeGameMode.h"
#include "QuinnsEscapeCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Controllers/QuinnPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

#include "UI/Level0HUD.h"
#include "Game/QuinnGameState.h"
#include "World/EndLevelTrigger.h"
#include "Characters/QuinnCharacter.h"
#include "QEPlayerSaveData.h"

AQuinnsEscapeGameMode::AQuinnsEscapeGameMode()
{
	m_isLevelOver = false;

	// Set default player controller to custom one
	PlayerControllerClass = AQuinnPlayerController::StaticClass();
	GameStateClass = AQuinnGameState::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/QuinnsEscape/Player/BP_QuinnCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to determine DefaultPawnClass! PlayerPawn path is incorrect"));
	}

	// Set HUD class from Blueprint
	static ConstructorHelpers::FClassFinder<AHUD> Level0HUDBPClass(TEXT("/Game/QuinnsEscape/Blueprints/BP_Level0HUD"));
	if (Level0HUDBPClass.Class != NULL)
	{
		HUDClass = Level0HUDBPClass.Class;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to determine HUDClass! Level0HUDBPClass path is incorrect"));
	}
}

void AQuinnsEscapeGameMode::StartPlay()
{
	Super::StartPlay();

	// Bind level complete and failed events
	TryBindLevelCompleteEvent();
	TryBindLevelFailedEvent();
}

void AQuinnsEscapeGameMode::OnGameOver(bool didCompleteLevel)
{
	if (m_isLevelOver)
	{
		return;
	}

	m_isLevelOver = true;

	float completeTimeSeconds = 0;

	// Get player controller and get HUD from PC.
	APlayerController* pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (!IsValid(pc))
	{
		return;
	}

	// Display input for player
	pc->SetIgnoreMoveInput(true);
	pc->SetIgnoreLookInput(true);

	AHUD* hud = pc->GetHUD();
	// Set state to level complete
	if (ALevel0HUD* lvlHUD = Cast<ALevel0HUD>(hud))
	{
		lvlHUD->SetHUDDisplayState(didCompleteLevel ? EHUDState::LevelComplete : EHUDState::LevelFailed);
	}

	// Cast controller char to Quinn
	AQuinnCharacter* quinn = Cast<AQuinnCharacter>(pc->GetCharacter());

	// get game state and cast
	AQuinnGameState* quinnGameState = Cast<AQuinnGameState>(GetWorld()->GetGameState());

	if (IsValid(quinnGameState) && didCompleteLevel)
	{
		double completeRewardScore = 50;
		float averageCompletionSeconds = 60;	// 1 min
		// Determine level complete multiplier
		float completeMultiplier = averageCompletionSeconds / quinn->GetCharacterAliveDuration();
		// Use Log2 to get multiplier
		/*	Add determined value to 2 as to always give a positive multiplier
			Log2(2) = 1, Log2(3) = 1.5, Log2(4) = 2, etc
		*/
		completeMultiplier = 2 + FMath::FloorLog2(completeMultiplier);
		// Multiply complete score by bonus multiplier
		double totalRewardScore = completeRewardScore * completeMultiplier;
		// Add multiplied score to total
		quinnGameState->AddScore(totalRewardScore); 
	}

	if (IsValid(quinn) && IsValid(quinnGameState))
	{
		// Process current game to save file
		FQESingleGameData gameData(quinn->GetCharacterAliveDuration(), quinnGameState->GetScore(), didCompleteLevel);
		ProcessGameDataToSaveFile(gameData);
	}
}

void AQuinnsEscapeGameMode::OnLevelComplete(AActor* endingTrigger, ACharacter* quinnChar)
{
	OnGameOver(true);
	UE_LOG(LogTemp, Log, TEXT("Level ended through trigger '%s' by '%s'"), *endingTrigger->GetName(), *quinnChar->GetName());
}

void AQuinnsEscapeGameMode::OnQuinnDeath()
{
	OnGameOver(false);
	UE_LOG(LogTemp, Log, TEXT("Level ended through player death"));
}

bool AQuinnsEscapeGameMode::TryBindLevelCompleteEvent()
{
	// Get list of all end level triggers in level
	TArray<AActor*> allEndLevelTriggers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEndLevelTrigger::StaticClass(), allEndLevelTriggers);

	// Validate we have more than none
	if (allEndLevelTriggers.Num() > 0)
	{
		// Iterate through all found actors to bind to event
		for (AActor* triggerActor : allEndLevelTriggers)
		{
			// Cast from AActor to AEndLevelTrigger
			if (AEndLevelTrigger* endTrigger = Cast<AEndLevelTrigger>(triggerActor))
			{
				// Bind event to function
				endTrigger->OnQuinnBeginOverlapTrigger.AddDynamic(this, &AQuinnsEscapeGameMode::OnLevelComplete);
			}
		}

		// Log and return true for success
		UE_LOG(LogTemp, Log, TEXT("Detecting end of level from '%d' EndLevelTrigger(s)"), allEndLevelTriggers.Num());
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to find any AEndLevelTrigger actors! Won't be able to end the level"));
	}

	// Failed to bind any events
	return false;
}

bool AQuinnsEscapeGameMode::TryBindLevelFailedEvent()
{
	ACharacter* playerChar = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (IsValid(playerChar))
	{
		// Cast to Quinn and bind death event
		AQuinnCharacter* quinn = Cast<AQuinnCharacter>(playerChar);
		quinn->OnCharacterDied.AddDynamic(this, &AQuinnsEscapeGameMode::OnQuinnDeath);
	}

	return false;
}

void AQuinnsEscapeGameMode::ProcessGameDataToSaveFile(FQESingleGameData gameData)
{
	// Load save game if exists
	UQEPlayerSaveData* saveGame = UQEPlayerSaveData::GetCurrentSaveData();
	if (IsValid(saveGame))
	{
		saveGame->AddGameToHistory(gameData);

		UE_LOG(LogTemp, Log, TEXT("Saved game to existing save game"));
	}
	else
	{
		// Create new save data and add first game
		saveGame = Cast<UQEPlayerSaveData>(UGameplayStatics::CreateSaveGameObject(UQEPlayerSaveData::StaticClass()));
		// Add to history
		saveGame->AddGameToHistory(gameData);

		UE_LOG(LogTemp, Log, TEXT("Created new save file and saved to slot"));
	}

	// check save data is valid
	if (IsValid(saveGame))
	{
		// Validate game for high score
		bool isNewHighScore = saveGame->ValidateHighScore(gameData);
		if (isNewHighScore)
		{
			UE_LOG(LogTemp, Log, TEXT("Score of '%f' is a new high score!"), gameData.Score);
		}

		// Print all high scores. ToDo: Show this in UI
		// Get scores and check more than 0
		TArray<FQESingleGameData> highScores = saveGame->GetHighScores();
		if (highScores.Num() > 0)
		{
			UE_LOG(LogTemp, Log, TEXT("--- HIGH SCORES ---"));
			// Iterate though all and print
			for (int i = 0; i < highScores.Num(); i++)
			{
				// Parse complete date time
				FDateTime dt;
				FDateTime::ParseIso8601(*(highScores[i].CompleteDateTime), dt);

				UE_LOG(LogTemp, Log, TEXT("High Score '%d': DateTime: '%s', Score: '%f', Duration: '%f'"), i + 1, *dt.ToString(), highScores[i].Score, highScores[i].CompleteSeconds);
			}
			UE_LOG(LogTemp, Log, TEXT("--- END HIGH SCORES ---"));
		}
	}

	// Save to slot
	UQEPlayerSaveData::SaveData(saveGame);
}