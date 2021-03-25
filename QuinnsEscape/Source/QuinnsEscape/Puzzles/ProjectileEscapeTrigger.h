// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "ProjectileEscapeTrigger.generated.h"

/**
 * Trigger box that detects if a projectile has collided with
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
