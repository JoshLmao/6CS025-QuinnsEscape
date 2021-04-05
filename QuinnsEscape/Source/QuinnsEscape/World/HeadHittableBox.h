// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HeadHittableBox.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHittableBoxRecievedHeadHitSignature, ACharacter*, character);

/*
*	Simple box with a collider below to detect any characters that perform a "head hit" on this box.
*/
UCLASS()
class QUINNSESCAPE_API AHeadHittableBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHeadHittableBox();

	/*
	*	VARIABLES
	*/
protected:
	// Collider to be placed below the box to detect collision and spawning powerup
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BeneathBoxCollider;

	// Mesh component for the powerup box
	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* MeshComponent;

	// Audio component of the box
	UPROPERTY(VisibleAnywhere)
	class UAudioComponent* AudioComponent;

	// Sound to play when the box has been hit with the player's head
	UPROPERTY(EditAnywhere)
	class USoundBase* HitSound;

private:
	// Amount of times character has hit box with their head
	int m_recievedHeadHitCount;

	/*
	*	EVENTS
	*/
public:
	// Triggered when a character collides it's head with this head hittable box
	UPROPERTY(BlueprintAssignable)
	FHittableBoxRecievedHeadHitSignature OnRecievedHeadHit;

	/*
	*	METHODS
	*/
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Displays the give text content on the side of the box and returns the created text actor
	class ATextRenderActor* CreateBoxWithText(FString content, FColor color = FColor::Black);

	// Sets the text color of the given text actor
	void SetTextColor(class ATextRenderActor* textActor, FColor color);

private:
	// Begin overlap callback for the box component
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
