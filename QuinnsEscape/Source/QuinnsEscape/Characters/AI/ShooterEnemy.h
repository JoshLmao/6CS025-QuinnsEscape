

#pragma once

#include "CoreMinimal.h"
#include "Characters/HealthCharacter.h"
#include "ShooterEnemy.generated.h"

// Enum of all potential states of AShooterEnemy
UENUM()
enum EShooterStates
{
	Idle		= 0		UMETA(DisplayName = "Idle"),
	Chasing		= 1		UMETA(DisplayName = "Chase"),
	Shooting	= 2		UMETA(DisplayName = "Shoot"),
	Dead		= 3		UMETA(DisplayName = "Dead"),
};

// All potential states of a finite state machine (FSM)
UENUM()
enum EFSMStates
{
	Begin	= 0		UMETA(DisplayName = "Begin"),
	Tick	= 1		UMETA(DisplayName = "Tick"),
};

const FString ShooterEnemyProperties;

/**
 *	Base shooter enemy.
 *	Enemy's aim is to wait until player is within a detection range, 
 *	approach the player for shooting range and fire at player every X duration
 */
UCLASS()
class QUINNSESCAPE_API AShooterEnemy : public AHealthCharacter
{
	GENERATED_BODY()
	
public:
	// Default constructor
	AShooterEnemy();

	/*
	*	VARIABLES
	*/
public:
	// Amount of units the target can enter before enemy starts to approach
	UPROPERTY(EditAnywhere, Category = ShooterEnemyProperties)
	float DetectionRadius;
	// Amount of units this enemy should be from the target to start firing
	UPROPERTY(EditAnywhere, Category = ShooterEnemyProperties)
	float ShootingRadius;

	// Amount of time in seconds in between shooting
	UPROPERTY(EditAnywhere, Category = ShooterEnemyProperties)
	float ShootingInteval;
	// Speed of the projectiles shot by this enemy
	UPROPERTY(EditAnywhere, Category = ShooterEnemyProperties)
	float ProjectileSpeed;
	// Range of the amount of damage a projectile will deal to its target. 
	// Actual damage will be a random number between these values
	UPROPERTY(EditAnywhere, Category = ShooterEnemyProperties)
	FFloatRange ProjectileDamage;

	// Which class should this enemy target
	UPROPERTY(EditAnywhere, Category = ShooterEnemyProperties)
	TSubclassOf<class ACharacter> TargetClass;

protected:
	// Projectile class to use when shooting
	UPROPERTY(EditAnywhere, Category = ShooterEnemyProperties)
	TSubclassOf<class AProjectileBase> ProjectileClass;

	// Reference to target character in scene
	ACharacter* TargetCharacter;

private:
	// Timer handle for firing at player when in range
	FTimerHandle m_fireDelayHandle;
	// Current state of the enemy
	UPROPERTY(VisibleAnywhere)
	TEnumAsByte<EShooterStates> m_currentState;
	// Current state of the FSM
	EFSMStates m_fsmState;
	// Current controller casted to an AI controller
	class AAIController* AIController;

	/*
	*	METHODS
	*/
public:
	// Begin play
	virtual void BeginPlay() override;
	// Tick
	virtual void Tick(float deltaTime) override;

protected:
	// Override
	virtual bool OnCharacterDeath() override;

private:
	// Primary update for running the finite state machine
	void FSMUpdate(float deltaTime);
	// Set the FSM to a new state
	void SetShooterState(EShooterStates newState);

	// Begin FSM state for Idle
	void Idle_Begin();
	// Tick FSM state for Idle
	void Idle_Tick(float deltaTime);
	// Begin FSM state for Chasing
	void Chasing_Begin();
	// Tick FSM state for Chasing
	void Chasing_Tick(float deltaTime);
	// Begin FSM state for Shooting
	void Shooting_Begin();
	// Tick FSM state for Shooting
	void Shooting_Tick(float deltaTime);
	// Begin FSM state for Dead
	void Dead_Begin();
	// Tick FSM state for Dead
	void Dead_Tick(float deltaTime);

	// Shoots at the current target character
	void ShootAtTarget();
};
