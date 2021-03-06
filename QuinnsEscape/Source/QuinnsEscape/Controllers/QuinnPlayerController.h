// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "QuinnPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class QUINNSESCAPE_API AQuinnPlayerController : public APlayerController
{
	GENERATED_BODY()

	AQuinnPlayerController();

protected:
	// Quinn character ptr reference
	class AQuinnCharacter* QuinnCharacter;

protected:
	// Begin PlayerController interface
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	virtual void OnPossess(APawn* aPawn) override;
	virtual void OnUnPossess() override;
	// End PlayerController interface

	// Fire action button is released
	void OnFireReleased();
	// Slam action button is released
	void OnSlamReleased();
	// Axis Input, Move Right
	void OnAxisMoveRight(float value);
	// Action Input - Jump Pressed
	void OnActionJump();
	// Action Input - Jump Released
	void OnActionStopJumping();

	// Debug: Delete all save game data
	void OnDeleteAllSaveGameData();
};
