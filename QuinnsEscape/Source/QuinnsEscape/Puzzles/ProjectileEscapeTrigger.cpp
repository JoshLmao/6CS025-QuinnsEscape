// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileEscapeTrigger.h"
#include "Components/ShapeComponent.h"
#include "../Projectiles/ProjectileBase.h"
#include "PuzzleEscapeDoor.h"

AProjectileEscapeTrigger::AProjectileEscapeTrigger()
{
	// Get component and bind overlap event
	if (this->GetCollisionComponent() != nullptr)
	{
		this->GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &AProjectileEscapeTrigger::OnTriggerBeginOverlap);
	}
}

void AProjectileEscapeTrigger::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Detect other actor is a projectile
	if (OtherActor->IsA(AProjectileBase::StaticClass()))
	{
		// Check escape door has been set
		if (IsValid(EscapeDoor))
		{
			EscapeDoor->RaiseEscape();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("No escape door set has been on '%s'!"), *this->GetName());
		}
	}
}
