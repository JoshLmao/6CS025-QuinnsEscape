// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/HealthCharacter.h"
#include "QuinnCharacter.generated.h"



/**
 * Quinn Character:
 * Main player character, Quinn
 */
UCLASS()
class QUINNSESCAPE_API AQuinnCharacter : public AHealthCharacter
{
	GENERATED_BODY()
	
public:
	AQuinnCharacter();

	/*
	*	VARIABLES
	*/
public:
	// Amount of lives the player currently has
	int CurrentLives;

protected:
	// Side view camera 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;

	// Camera boom positioning the camera beside the character
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	// Total cooldown in seconds in between firing each projectile
	UPROPERTY(EditAnywhere)
	float FireTotalCooldown;
	// Speed that the projectile is fired at
	UPROPERTY(EditAnywhere)
	float ProjectileSpeed;
	// Min and Max damage values to randomly select between when a projectile deals damage
	UPROPERTY(EditAnywhere)
	FVector2D ProjectileDamageMinMax;
	// Total cooldown in seconds for the slam ability
	UPROPERTY(EditAnywhere)
	float SlamTotalCooldown;
	// Amount of damage to deal to an enemy if the player lands on their head
	UPROPERTY(EditAnywhere)
	float HeadJumpDamage;
	// Amount multiplier to add to the HeadJumpDamage if the player slams onto an enemy head
	UPROPERTY(EditAnywhere)
	float SlamMultiplier;

	// How many lives the player starts with
	UPROPERTY(EditAnywhere)
	int StartLives;

	// Last checkpoint the player passed through
	class ACheckpoint* LastCheckpoint;

private:
	// Actor to use to as a projectile fired from the character
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectileBase> ProjectileActor;
	// Sphere Component placed below character to detect stomps
	UPROPERTY(EditAnywhere)
	class USphereComponent* StompSphereComponent;

	// Current cooldown of firing a projectile
	float m_fireCooldown;
	// Current cooldown of slam ability
	float m_slamCooldown;

	// Is the character currently slamming down to the ground
	bool m_isSlamingGround;
	// Has any damage been applied during this jump
	bool m_hasAppliedDmgThisJump;
	
	// Is the character currently unable to take damage?
	bool m_isInvulnerable;
	// Timer handler for invulernability for a duration
	FTimerHandle m_invulnerabilityTimerHandle;

	// Multiplier amount to times any damage dealt by this character
	float m_currentDmgMultiplier;
	// Timer handler for a dmg multiplier for a duration
	FTimerHandle m_dmgMultiplierTimerHandle;

	/*
	*	METHODS
	*/
public:
	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	// Fires a projectile in the destined direction
	void FireProjectile(FVector direction);
	// Slams into the ground below the character
	void SlamGround();

	// Gets the current lives of Quinn
	int GetCurrentLives();
	// Adds an amount of lives to the current lives. CurrentLive's cannot supass the total lives
	void AddLives(int amount);
	// Gets the total lives of Quinn
	int GetTotalLives();

	// Gets the last checkpoint that Quinn passed through
	class ACheckpoint* GetLastCheckpoint();
	// Sets the last active checkpoint the character passed through
	void SetCheckpoint(class ACheckpoint* checkpoint);

	// Set if the character can take damage or not (invulnerability)
	void SetIsInvulnerable(bool isInvulnerable);
	// Get if the character can take damage or not (invulnerability)
	bool GetIsInvulnerable();
	// Sets invulnerability to true for a duration
	void SetIsInvulnerableForDuration(int duration);

	// Sets the damage multiplier to the given multiplier for duration of seconds
	void SetDamageMultiplierForDuration(float multiplier, int duration);
	// Gets the current damage multiplier on the character
	float GetDamageMultiplier();
	// Set the current damage multiplier
	void SetDamageMultiplier(float multiplier);

protected:
	// BeginPlay function called on Actor begin
	virtual void BeginPlay() override;
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// Tick function called every frame
	virtual void Tick(float DeltaTime) override;
	// Jump function to force character in air
	virtual void Jump() override;
	// Called when character takes damage
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// Called when the character dies, runs out of health
	virtual bool OnCharacterDeath() override;

	// Called for side to side input
	void MoveRight(float Val);

private:
	// Callback function for the StompSphereComponent OnComponentBeginOverlap
	UFUNCTION()
	void OnStompCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Invulnerability callback function once timer has finished
	UFUNCTION()
	void OnInvulnerabilityDurationExpired();

	// Damage Multiplier callback function once timer has finished
	UFUNCTION()
	void OnDmgMultiplierDurationExpired();

	// Clears any effects, such as bonuses or debuffs, currently applied to the character
	void ClearCharacterEffects();
};
