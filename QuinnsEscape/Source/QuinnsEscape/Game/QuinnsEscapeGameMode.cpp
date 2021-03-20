// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "QuinnsEscapeGameMode.h"
#include "QuinnsEscapeCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Controllers/QuinnPlayerController.h"
#include "Kismet/GameplayStatics.h"

#include "UI/Level0HUD.h"
#include "Game/QuinnGameState.h"
#include "World/EndLevelTrigger.h"
#include "Characters/QuinnCharacter.h"

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

void AQuinnsEscapeGameMode::OnLevelComplete(AActor* endingTrigger, ACharacter* quinnChar)
{
	if (m_isLevelOver)
	{
		return;
	}

	m_isLevelOver = true;
	UE_LOG(LogTemp, Log, TEXT("Level ended through trigger '%s' by '%s'"), *endingTrigger->GetName(), *quinnChar->GetName());

	// Get player controller and get HUD from PC.
	APlayerController* pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (IsValid(pc))
	{
		AHUD* hud = pc->GetHUD();
		// Set state to level complete
		if (ALevel0HUD* lvlHUD = Cast<ALevel0HUD>(hud))
		{
			lvlHUD->SetHUDDisplayState(EHUDState::LevelComplete);
		}

		// Display input for player
		pc->SetIgnoreMoveInput(true);
		pc->SetIgnoreLookInput(true);
	}
}

void AQuinnsEscapeGameMode::OnQuinnDeath()
{
	m_isLevelOver = true;
	UE_LOG(LogTemp, Log, TEXT("Level ended through player death"));

	// Get player controller and get HUD from PC.
	APlayerController* pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (IsValid(pc))
	{
		AHUD* hud = pc->GetHUD();
		// Set state to level failed
		if (ALevel0HUD* lvlHUD = Cast<ALevel0HUD>(hud))
		{
			lvlHUD->SetHUDDisplayState(EHUDState::LevelFailed);
		}
	}
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