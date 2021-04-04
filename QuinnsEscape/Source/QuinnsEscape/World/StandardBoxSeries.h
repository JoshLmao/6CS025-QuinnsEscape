

#pragma once

#include "CoreMinimal.h"
#include "World/StandardBox.h"
#include "StandardBoxSeries.generated.h"

/**
 * 
 */
UCLASS()
class QUINNSESCAPE_API AStandardBoxSeries : public AStandardBox
{
	GENERATED_BODY()
	
public:
	// Default constructor
	AStandardBoxSeries();

	/*
	*	VARIABLES
	*/
public:
	// Amount of standard boxes to spawn to the right of this box
	UPROPERTY(EditAnywhere)
	int SeriesCount;

protected:
	TArray<AActor*> SeriesBoxes;

	/*
	*	METHODS
	*/
public:
	virtual void BeginPlay() override;
};
