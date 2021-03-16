// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerupPickup.generated.h"

/*
* Base class for a pickup-able powerup that the main player can pick up.
*/
UCLASS()
class QUINNSESCAPE_API APowerupPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APowerupPickup();

	/*
	*	VARIABLES
	*/
public:
	// Mesh to set on the static mesh component
	class UStaticMesh* Mesh;

	// Speed at which the powerup is rotated, spinning round
	UPROPERTY(EditAnywhere, Category = "Spin Animation")
	float RotationSpeed;

	// Speed at which to float up and down
	UPROPERTY(EditAnywhere, Category = "Spin Animation")
	float FloatSpeed;

	// Amount of movement that happens on the Z axis on the spin/float animation
	UPROPERTY(EditAnywhere, Category = "Spin Animation")
	float ZMoveAmount;

protected:
	// Root mesh component for visual
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* StaticMeshComponent;
	// Box component to detect overlap with characters
	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxTrigger;

private:
	bool m_animGoUp;
	float m_minZ;
	float m_maxZ;
	
	/*
	*	METHODS
	*/
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Apply the powerup effects to the collided pawn.
	virtual void ApplyEffects(APawn* collidedPawn);
	 
	// Gets the game state and casts it to AQuinnGameState
	class AQuinnGameState* GetQuinnGameState();

private:
	// Called when BoxTrigger begins overlap
	UFUNCTION()
	void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Performs spin animation code
	void DoSpinAnimation(float deltaTime);
};
