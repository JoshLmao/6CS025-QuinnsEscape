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
	// Amount of score to reward the player with for passing the checkpoint
	UPROPERTY(EditAnywhere, Category = "Checkpoint Details")
	int ScoreReward;

	// Box extent of the checkpoint trigger
	UPROPERTY(EditAnywhere, Category = "Checkpoint Details")
	FVector BoxExtent;
	UPROPERTY(EditAnywhere, Category = "Checkpoint Details")
	// Static mesh to use on the checkpoint
	class UStaticMesh* CheckpointMesh;
	// Sound to p[lay when the player reaches and activates
	UPROPERTY(EditAnywhere, Category = Sounds)
	class USoundBase* ActivatedSound;

private:
	// Root box component to use as trigger
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* m_pointTrigger;
	// Mesh component of the checkpoint
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* m_staticMesh;
	// Audio component to play sounds
	UPROPERTY(VisibleAnywhere)
	class UAudioComponent* m_audioComponent;
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

	void OnPlayerPassCheckpoint(AActor* player);

};
