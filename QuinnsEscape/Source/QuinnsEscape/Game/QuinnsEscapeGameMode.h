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

private:
	bool m_isLevelOver;

	/*
	*	METHODS
	*/
protected:
	// Start/Begin play
	virtual void StartPlay() override;

private:
	// Reciever of event from EndLevelTrigger
	UFUNCTION()
	void OnLevelComplete(AActor* endingTrigger, ACharacter* quinnChar);

	UFUNCTION()
	void OnQuinnDeath();

	bool TryBindLevelCompleteEvent();
	bool TryBindLevelFailedEvent();
};



