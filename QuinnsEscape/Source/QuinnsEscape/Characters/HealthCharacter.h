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
public:
	// Amount of score to reward when character gets hit with a regular attack
	UPROPERTY(EditAnywhere, Category = "Scoring")
	double HitScore;
	// Amount of score to reward when this character dies
	UPROPERTY(EditAnywhere, Category = "Scoring")
	double KilledScore;

protected:
	// Sound to play when the character runs out of health (dies)
	UPROPERTY(EditAnywhere, Category = Sounds)
	class USoundBase* DeathSound;
	// Sound to play when the character gets hit. Will only play when off cooldown, if HitSoundCooldown is more than 0
	UPROPERTY(EditAnywhere, Category = Sounds)
	class USoundBase* HitSound;
	// Sound to play when the character performs a jump (or launched in the air)
	UPROPERTY(EditAnywhere, Category = Sounds)
	class USoundBase* JumpSound;
	// Time in seconds for the HitSound to be on cooldown and not be played again until this cooldown is over
	UPROPERTY(EditAnywhere, Category = Sounds)
	float HitSoundCooldown;

	// Main audio component of the character
	UPROPERTY(VisibleAnywhere)
	class UAudioComponent* AudioComponent;

private:
	// Current health of the character
	float m_currentHealth;
	// Total health of the character
	float m_totalHealth;
	// If this character can be stomped
	bool m_canRecieveStomp;
	// Should the character be destroyed when falling out of world
	bool m_shouldDestroyOnFall;
	// Time in seconds the character has been alive for
	float m_aliveTimeSecs;
	// Current cooldown of the hit sound
	float m_hitSoundCurrentCooldown;

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
	// Get if the character should be destroyed when falling out of level
	bool GetDestroyOnFall();
	// Set if the character should be destroyed when falling out of level
	void SetDestroyOnFall(bool shouldDestroy);

	// Gets if the current health character is alive or dead
	bool GetIsCharacterAlive();

	// Gets the duration in seconds the character has been alive
	float GetCharacterAliveDuration();

	// Can this character recieve being jumped on the head of
	bool CanRecieveStomp();
	// Set if this character can be stomped
	void SetCanRecieveStomp(bool canBeStomped);
	// Deal a stomp to this character and return if successfuly dealth damage
	bool TakeStomp(float damage);

	// Gets the amount of score to reward to the other character for a regular hit to this character
	double GetHitScoreReward();
	// Sets the amount of score to reward the other character with when hit with a regular attack
	void SetHitScoreReward(double score);
	// Gets the amount of score to reward when this character dies
	double GetKilledScoreReward();
	// Sets the amount of score to reward when this character dies
	void SetKilledScoreReward(double score);

protected:
	// Overridable method called when character has ran out of health. Returns bool of if the character should die or not
	virtual bool OnCharacterDeath();
	// Override falling out of the world
	virtual void FellOutOfWorld(const class UDamageType& dmgType) override;

	virtual void LaunchCharacter(FVector LaunchVelocity, bool bXYOverride, bool bZOverride) override;
	virtual void Jump() override;

	// Gets a FVector location that is in the middle of the character.
	// Also adds the given offset amount on the Y axis (left/right in 2D). 
	// Offset can be a negative to get middle-left, positive for middle-right
	FVector GetHalfLocationWithOffset(float yOffset);

private:
	// Deals an amount of damage to the character
	void DealDamage(float damage);

};
