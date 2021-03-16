// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "QuinnsEscapeGameMode.h"
#include "QuinnsEscapeCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Controllers/QuinnPlayerController.h"

#include "UI/Level0HUD.h"
#include "Game/QuinnGameState.h"

AQuinnsEscapeGameMode::AQuinnsEscapeGameMode()
{
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
