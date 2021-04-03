


#include "VelocityLinkProxy.h"
#include "GameFramework/Character.h"

AVelocityLinkProxy::AVelocityLinkProxy()
{
	// Default values
	JumpDuration = 1.0f;

	bSmartLinkIsRelevant = true;
	this->OnSmartLinkReached.AddDynamic(this, &AVelocityLinkProxy::SmartLinkReached);
}

void AVelocityLinkProxy::BeginPlay()
{
	Super::BeginPlay();
}

void AVelocityLinkProxy::LaunchCharacter(AActor* MovingActor, const FVector& DestinationPoint)
{
	// Check if need to add velocity if destination is higher than current location
	if (MovingActor->GetActorLocation().Z < DestinationPoint.Z)
	{
		// Check its a character so we can add velocity/jump
		if (MovingActor->IsA(ACharacter::StaticClass()))
		{
			ACharacter* character = Cast<ACharacter>(MovingActor);

			// Determine velocity to launch character and launch
			FVector velocity = CalculateVelocity(MovingActor->GetActorLocation(), DestinationPoint, JumpDuration);
			character->LaunchCharacter(velocity, true, true);
		}
	}
}

void AVelocityLinkProxy::SmartLinkReached(AActor* MovingActor, const FVector& DestinationPoint)
{
	// Check if need to add velocity if destination is higher than current location
	LaunchCharacter(MovingActor, DestinationPoint);
}

FVector AVelocityLinkProxy::CalculateVelocity(FVector startingLoc, FVector destinationLoc, float duration)
{
	// Determine X
	float x = destinationLoc.X - startingLoc.X;
	x = x / duration;

	// Determine Y
	float y = destinationLoc.Y - startingLoc.Y;
	y = y / duration;

	// Speed of Gravity is 9.82^2. Convert to CM for Unreal
	float speedOfGravityCM = 982.0f;
	float add = FMath::Square(JumpDuration) * -0.5 * speedOfGravityCM;

	// Determine Z
	float z = startingLoc.Z + add;
	z = destinationLoc.Z - z;
	z = z / duration;

	// Return vector as velocity
	return FVector(x, y, z);
}


