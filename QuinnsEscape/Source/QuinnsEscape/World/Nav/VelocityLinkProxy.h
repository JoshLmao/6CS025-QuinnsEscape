

#pragma once

#include "CoreMinimal.h"
#include "Navigation/NavLinkProxy.h"
#include "VelocityLinkProxy.generated.h"

/**
 *	Nav Link Proxy that uses velocity to launch a character to the other proxy point
 */
UCLASS()
class QUINNSESCAPE_API AVelocityLinkProxy : public ANavLinkProxy
{
	GENERATED_BODY()
	
public:
	// Default constructor
	AVelocityLinkProxy();

	// How long the velocity jump will last when climbing a proxy link (From a low Z to a high Z)
	UPROPERTY(EditAnywhere)
	float JumpDuration;

	/*
	*	METHODS
	*/
public:
	// Launches the actor to the destination point, if it's a character and if the destination Z is higher than the moving actor's Z
	UFUNCTION(BlueprintCallable)
	void LaunchCharacter(AActor* MovingActor, const FVector& DestinationPoint);

protected:
	void BeginPlay() override;

private:
	// Reciever event for when a smart link has been reached
	UFUNCTION()
	void SmartLinkReached(AActor* MovingActor, const FVector& DestinationPoint);

	// Calculates the required velocity
	FVector CalculateVelocity(FVector startingLoc, FVector destinationLoc, float duration);
};
