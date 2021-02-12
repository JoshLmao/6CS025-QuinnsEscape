// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectileBase.generated.h"

/**
* Projectile Base:
* Base class for any projectiles
*/
UCLASS()
class QUINNSESCAPE_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectileBase();

#pragma region Child Class Overrides
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
#pragma endregion

	/*
	*	VARIABLES
	*/
private:
	// Root capsule component to handle colliding
	class UCapsuleComponent* m_rootCapsuleComponent;
	// Component to handle moving actor as a projectile
	class UProjectileMovementComponent* m_projectileMovementComponent;
	// Mesh representation for the projectile
	class UStaticMeshComponent* m_meshComponent;

	// Amount of damage to deal out to the recieveing actor
	float m_damage;
	// Class to ignore in overlap
	class UClass* m_ignoreClass;

	/*
	*	METHODS
	*/
public:
	// Get the current speed of the projectile
	float GetSpeed();
	// Set the speed of the projectiles
	void SetSpeed(float speed);

	// Amount of damage to deal to the recieveing actor
	float GetDamage();
	// Set the amount of damage to deal to overlapped actor
	void SetDamage(float damage);

	// Fires the projectile in the direction
	void FireInDirection(FVector fireDirection);
	
	// Add an actor to ignore on the root capsule component
	void AddActorToIgnore(AActor* actor);

protected:
	// Called when collider begins to overlaps any actor
	UFUNCTION()
	virtual void OnColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	// Called when collider ends overlapping any actor
	UFUNCTION()
	virtual void OnColliderEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
