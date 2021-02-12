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
	// Begin PlayerController interface
	virtual void BeginPlay() override;
	virtual void PlayerTick(float DeltaTime) override;
	virtual void SetupInputComponent() override;
	// End PlayerController interface

	void OnFireReleased();
};
