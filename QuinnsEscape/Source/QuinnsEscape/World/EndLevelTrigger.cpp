// Fill out your copyright notice in the Description page of Project Settings.


#include "EndLevelTrigger.h"
#include "Engine/TriggerBox.h"
#include "Components/ShapeComponent.h"
#include "../Characters/QuinnCharacter.h"

// Sets default values
AEndLevelTrigger::AEndLevelTrigger()
{
	// Get component and bind overlap event
	if (GetCollisionComponent() != nullptr)
	{
		GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &AEndLevelTrigger::OnTriggerBeginOverlap);
	}
}

void AEndLevelTrigger::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check other actor is Quinn
	if (OtherActor->IsA(AQuinnCharacter::StaticClass()))
	{
		// Cast to ACharacter and broadcast event
		AQuinnCharacter* quinn = Cast<AQuinnCharacter>(OtherActor);
		if (OnQuinnBeginOverlapTrigger.IsBound())
		{
			OnQuinnBeginOverlapTrigger.Broadcast(this, quinn);
		}
	}
}

