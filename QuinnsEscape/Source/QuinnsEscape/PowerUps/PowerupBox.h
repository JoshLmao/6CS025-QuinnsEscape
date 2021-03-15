// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerupBox.generated.h"

/*
* Box that can be hit from below to spawn a powerup
*/
UCLASS()
class QUINNSESCAPE_API APowerupBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APowerupBox();

	/*
	*	VARIABLES
	*/
public:
	// Array of all powerups that can be spawned
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class APowerupPickup>> AllPowerups;

	// Relative location of where to spawn a powerup to the box
	UPROPERTY(EditAnywhere)
	FVector PowerupSpawnLocation;

	// Maximum amount of powerups the box should create
	UPROPERTY(EditAnywhere)
	int MaxPowerupSpawnCount;

protected:
	// Collider to be placed below the box to detect collision and spawning powerup
	UPROPERTY(EditAnywhere)
	class UBoxComponent* BeneathBoxCollider;

	// Mesh component for the powerup box
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* MeshComponent;

	// Text component used to represent a character to show this box is breakable
	UPROPERTY(EditAnywhere)
	class ATextRenderActor* TextComponent;

private:
	// Current amount of spawned powerups
	int m_spawnedPowerupCount;

	/*
	*	METHODS
	*/
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Spawns a random powerup from the available powerups.
	// Returns if spawn was successful
	bool SpawnRandomPowerup();

	// Spawns a specific powerup above this powerup box
	// Returns if spawn was successful
	bool SpawnPowerup(TSubclassOf<class APowerupPickup> powerup);

	// Can this box spawn a powerup
	bool CanSpawnPowerup();

	// Creates the TextComponent to display text infront of box
	void CreateTextComponent();

};
