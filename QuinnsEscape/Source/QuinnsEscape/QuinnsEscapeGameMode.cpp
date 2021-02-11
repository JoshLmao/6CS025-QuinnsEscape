// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "QuinnsEscapeGameMode.h"
#include "QuinnsEscapeCharacter.h"
#include "UObject/ConstructorHelpers.h"

AQuinnsEscapeGameMode::AQuinnsEscapeGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/QuinnsEscape/Player/BP_QuinnCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
