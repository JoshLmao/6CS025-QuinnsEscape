// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Checkpoint.generated.h"

/*
*	Checkpoint to act as a safe place for the player to spawn at, if killed.
*/
UCLASS()
class QUINNSESCAPE_API ACheckpoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACheckpoint();

	/*
	*	VARIABLES
	*/
public:
	// Box extent of the checkpoint trigger
	UPROPERTY(EditAnywhere, Category = "Checkpoint Details")
	FVector BoxExtent;
	UPROPERTY(EditAnywhere, Category = "Checkpoint Details")
	// Static mesh to use on the checkpoint
	class UStaticMesh* CheckpointMesh;

private:
	// Root box component to use as trigger
	UPROPERTY(EditAnywhere)
	class UBoxComponent* m_pointTrigger;
	// Mesh component of the checkpoint
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* m_staticMesh;
	// Has the player passed through this checkpoint yet
	bool m_hasPassed;
	// Count to use to lerp and rotate mesh
	float m_lerpAlpha;

	/*
	*	METHODS
	*/
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float deltaTime) override;

private:
	UFUNCTION()
	void CheckpointBeginOverlap(class UPrimitiveComponent* OverlappedComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
