// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "QuinnsEscapeGameMode.h"
#include "QuinnsEscapeCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Controllers/QuinnPlayerController.h"

#include "Player\QuinnPlayerState.h"
#include "UI/Level0HUD.h"

AQuinnsEscapeGameMode::AQuinnsEscapeGameMode()
{
	// Set default player controller to custom one
	PlayerControllerClass = AQuinnPlayerController::StaticClass();
	PlayerStateClass = AQuinnPlayerState::StaticClass();
	HUDClass = ALevel0HUD::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/QuinnsEscape/Player/BP_QuinnCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
