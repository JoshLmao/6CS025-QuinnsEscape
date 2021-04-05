

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "PlayerEscapeTrigger.generated.h"

/**
 * 
 */
UCLASS()
class QUINNSESCAPE_API APlayerEscapeTrigger : public ATriggerBox
{
	GENERATED_BODY()

public:
	// Default construtor
	APlayerEscapeTrigger();

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
