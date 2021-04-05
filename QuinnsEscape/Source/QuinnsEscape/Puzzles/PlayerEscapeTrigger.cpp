


#include "PlayerEscapeTrigger.h"
#include "Components/ShapeComponent.h"

#include "Characters/QuinnCharacter.h"
#include "Puzzles/PuzzleEscapeDoor.h"

APlayerEscapeTrigger::APlayerEscapeTrigger()
{
	// Get component and bind overlap event
	if (this->GetCollisionComponent() != nullptr)
	{
		GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &APlayerEscapeTrigger::OnTriggerBeginOverlap);
	}
}

void APlayerEscapeTrigger::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Detect other actor is a projectile
	if (OtherActor->IsA(AQuinnCharacter::StaticClass()))
	{
		// Check escape door has been set
		if (IsValid(EscapeDoor))
		{
			// Open the escape
			EscapeDoor->OpenEscape();
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("No escape door set has been on '%s'!"), *this->GetName());
		}
	}
}
