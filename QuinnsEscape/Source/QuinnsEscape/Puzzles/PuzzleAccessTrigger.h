// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "PuzzleAccessTrigger.generated.h"

/**
 *	Access trigger for a entrance/exit to a puzzle.
 *	Teleports the player to a specific position, represented by a target point
 */
UCLASS()
class QUINNSESCAPE_API APuzzleAccessTrigger : public ATriggerBox
{
	GENERATED_BODY()
	
public:
	APuzzleAccessTrigger();

	/*
	*	VARIABLES
	*/
public:
	// Target point to teleport the player to once overlapping with this access trigger
	UPROPERTY(EditAnywhere)
	class ATargetPoint* TeleportTarget;

	// Z amount to lock the camera to when entering the access trigger. 0 to remain free/follow player
	UPROPERTY(EditAnywhere)
	float CameraLockZ;

	/*
	*	METHODS
	*/
private:
	// Overlap function
	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
