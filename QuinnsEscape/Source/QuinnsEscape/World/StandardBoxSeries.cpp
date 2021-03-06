


#include "StandardBoxSeries.h"
#include "Engine/World.h"

AStandardBoxSeries::AStandardBoxSeries()
{
	SeriesCount = 0;
}

void AStandardBoxSeries::BeginPlay()
{
	Super::BeginPlay();

	// Destroy any boxes if they exist
	if (SeriesBoxes.Num() > 0)
	{
		for (AActor* box : SeriesBoxes)
		{
			box->Destroy();
		}
	}

	// Iterate for count and spawn, set location
	for (int i = 1; i <= SeriesCount; i++)
	{
		// Calculate location
		float boxWidth = 100.0f;	// width of one AStandardBox 
		float padding = 15.0f;		// Total padding on both sides of the actor
		// Right vector requires inverting since we're designing level left to right (+Y to -Y)
		FVector rightVector = (-this->GetActorRightVector());
		FVector rightWithOffset = rightVector * ((i * boxWidth) + (i * padding));
		// Add offset to location
		FVector newLocation = this->GetActorLocation() + rightWithOffset;
		// Spawn new box
		AActor* box = GetWorld()->SpawnActor<AActor>(AStandardBox::StaticClass(), newLocation, FRotator(0, 0, 0));
		// Set as child of this actor
		FAttachmentTransformRules rules(EAttachmentRule::KeepWorld, true);
		box->AttachToActor(this, rules);

		if (!IsValid(box))
		{
			UE_LOG(LogTemp, Error, TEXT("Error creating box!"));
			continue;
		}

		SeriesBoxes.Add(box);
	}
}