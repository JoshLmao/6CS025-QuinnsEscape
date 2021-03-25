// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "ProjectileEscapeTrigger.generated.h"

/**
 *	Trigger box that detects if a projectile has collided with itself.
 *	Opens the targetted EscapeDoor to allow player to complete puzzle.
 *	Rewards the player with the amount of PointsReward on puzzle complete
 */
UCLASS()
class QUINNSESCAPE_API AProjectileEscapeTrigger : public ATriggerBox
{
	GENERATED_BODY()
	
public:
	AProjectileEscapeTrigger();
	
	/*
	*	VARIABLES
	*/
public:
	// Amount of points to reward the player
	UPROPERTY(EditAnywhere)
	float PointsReward;

	// Door to the exit of the puzzle
	UPROPERTY(EditAnywhere)
	class APuzzleEscapeDoor* EscapeDoor;

	/*
	*	METHODS
	*/
private:
	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
