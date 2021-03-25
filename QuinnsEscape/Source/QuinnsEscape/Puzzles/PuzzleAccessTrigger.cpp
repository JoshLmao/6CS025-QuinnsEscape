// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleAccessTrigger.h"
#include "Components/ShapeComponent.h"
#include "Engine/TargetPoint.h"
#include "../Characters/QuinnCharacter.h"

APuzzleAccessTrigger::APuzzleAccessTrigger()
{
	if (this->GetCollisionComponent() != nullptr)
	{
		this->GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &APuzzleAccessTrigger::OnTriggerBeginOverlap);
	}
}

void APuzzleAccessTrigger::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// if other actor is main character
	if (OtherActor->IsA(AQuinnCharacter::StaticClass()))
	{
		// Teleport to location if target point has been set
		if (IsValid(TeleportTarget))
		{
			OtherActor->SetActorLocation(TeleportTarget->GetActorLocation(), false);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("No target point has been set on '%s'"), *this->GetName());
		}
	}
}
