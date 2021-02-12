// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HealthCharacter.generated.h"

// Define death signature for event
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHealthCharDeathSignature);

/* 
* Health Character:
* A character that is capable of having health and recieveing damage that affects the health.
*/
UCLASS()
class QUINNSESCAPE_API AHealthCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AHealthCharacter();

#pragma region Child Class Overrides

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Called when character takes damage
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

#pragma endregion


	/*
	*	VARIABLES
	*/
private:
	// Current health of the character
	float m_currentHealth;
	// Total health of the character
	float m_totalHealth;


	/*
	*	EVENTS
	*/
public:
	// Event for when the character dies with no health remaining
	UPROPERTY(BlueprintAssignable)
	FHealthCharDeathSignature OnCharacterDied;


	/*
	*	METHODS
	*/
public:
	// Gets the current health of the character
	float GetCurrentHealth();
	// Sets the current health of the character. Is clamped between 0 and the total health
	void SetCurrentHealth(float health);
	// Gets the total health of the character
	float GetTotalHealth();
	// Set the total health of the character
	void SetTotalHealth(float health);

protected:
	// Overridable method called when character has died
	virtual void OnHealthCharacterDeath();

};