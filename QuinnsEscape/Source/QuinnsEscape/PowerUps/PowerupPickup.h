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

	// Should the spin animation spin left. False or unchecked to spin right
	UPROPERTY(EditAnywhere, Category = "Spin Animation")
	bool bRotateLeft;

	// Linear Color to set the light component to
	UPROPERTY(EditAnywhere)
	FLinearColor LightColor;

	// Sound to play when the player picks up this power-up
	UPROPERTY(EditAnywhere, Category = Sounds)
	class USoundBase* PickupSound;

protected:
	// Root mesh component for visual
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* StaticMeshComponent;
	// Box component to detect overlap with characters
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BoxTrigger;
	// Light around the power-up that emits a color around the power-up
	UPROPERTY(VisibleAnywhere)
	class UPointLightComponent* PointLight;
	// Audio component of the pickup
	UPROPERTY(VisibleAnywhere)
	class UAudioComponent* AudioComponent;

private:
	bool m_animGoUp;
	float m_minZ;
	float m_maxZ;
	bool m_isPickedUp;
	
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
#if WITH_EDITOR
	// When property gets updated in editor
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& e) override;
#endif
	// Gets the game state and casts it to AQuinnGameState
	class AQuinnGameState* GetQuinnGameState();

private:
	// Called when BoxTrigger begins overlap
	UFUNCTION()
	void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Performs spin animation code
	void DoSpinAnimation(float deltaTime);
};
