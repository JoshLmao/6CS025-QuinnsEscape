

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class QUINNSESCAPE_API AMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	// Default constructor
	AMainMenuGameMode();

	/*
	*	METHODS
	*/
public:
	virtual void BeginPlay() override;
};
