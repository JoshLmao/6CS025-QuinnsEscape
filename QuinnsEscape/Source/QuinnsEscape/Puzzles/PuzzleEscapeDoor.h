// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleEscapeDoor.generated.h"

/*
*	Door that blocks the escape of a puzzle. Able to raise to allow escape
*/
UCLASS()
class QUINNSESCAPE_API APuzzleEscapeDoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APuzzleEscapeDoor();

public:
	// Amount of units to move the escape door
	UPROPERTY(EditAnywhere)
	FVector RaiseAmount;

private:
	// Has the door already been raised?
	bool m_bIsRaised;
	// Is the door in the process of being raised
	bool m_bIsRaising;
	// Location to start lerping from (beginning position)
	FVector m_startLocation;

	// Mesh representation of the door
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* MeshComponent;

	/*
	*	METHODS
	*/
public:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Raises the escape door to allow the player to leave
	UFUNCTION(BlueprintCallable)
	void RaiseEscape();
};