

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StandardBox.generated.h"

UCLASS()
class QUINNSESCAPE_API AStandardBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStandardBox();

protected:
	class UStaticMeshComponent* BoxMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
