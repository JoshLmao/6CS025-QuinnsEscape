// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "EndLevelTrigger.generated.h"

// Define end level signature for event
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FEndLvlTriggerLevelEndedSignature, AActor*, endingTrigger, ACharacter*, quinnCharacter);


/*
*	Trigger to indicate the end of the level. 
*	Broadcasts an event once player passed through trigger
*/
UCLASS()
class QUINNSESCAPE_API AEndLevelTrigger : public ATriggerBox
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEndLevelTrigger();

	/*
	*	EVENTS
	*/
public:
	// OnEndLevel event for when quinn character passed through trigger
	UPROPERTY(BlueprintAssignable)
	FEndLvlTriggerLevelEndedSignature OnQuinnBeginOverlapTrigger;

	/*
	*	METHODS
	*/
private:
	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
