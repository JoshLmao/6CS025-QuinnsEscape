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

protected:
	// Root mesh component for visual
	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* StaticMeshComponent;
	// Box component to detect overlap with characters
	UPROPERTY(EditAnywhere)
	class UBoxComponent* BoxTrigger;
	
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
	virtual void ApplyEffects(APawn* pawn);
	 
private:
	// Called when BoxTrigger begins overlap
	UFUNCTION()
	void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
