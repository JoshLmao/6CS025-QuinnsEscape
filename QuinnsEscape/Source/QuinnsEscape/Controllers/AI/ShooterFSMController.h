

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ShooterFSMController.generated.h"

// Enum of all potential states of AShooterEnemy
UENUM()
enum EShooterStates
{
	Idle = 0		UMETA(DisplayName = "Idle"),
	Chasing = 1		UMETA(DisplayName = "Chase"),
	Shooting = 2		UMETA(DisplayName = "Shoot"),
	Dead = 3		UMETA(DisplayName = "Dead"),
};

// All potential states of a finite state machine (FSM)
UENUM()
enum EFSMStates
{
	Begin = 0		UMETA(DisplayName = "Begin"),
	Tick = 1		UMETA(DisplayName = "Tick"),
};

/**
 *	Finite State Machine controller for a Shooter NPC character.
 */
UCLASS()
class QUINNSESCAPE_API AShooterFSMController : public AAIController
{
	GENERATED_BODY()
	
public:
	// Default constructor
	AShooterFSMController();

	/*
	*	VARIABLES
	*/
protected:
	// Character currently being targetted by this controller
	ACharacter* TargetCharacter;

private:
	// Current state of the enemy
	UPROPERTY(VisibleAnywhere)
	TEnumAsByte<EShooterStates> m_currentState;
	// Current state of the FSM
	EFSMStates m_fsmState;
	// ShooterEnemy being controlled by this controller
	class AShooterEnemy* m_possesedCharacter;

	/*
	*	METHODS
	*/
public:
	// Begin play
	virtual void BeginPlay() override;
	// Tick
	virtual void Tick(float deltaTime) override;

	virtual void OnPossess(APawn* pawn) override;
	virtual void OnUnPossess() override;

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

	// Is the given actor within detection radius of this enemy
	bool IsInDetectionRadius(AActor* actor);
	// Is the given actor within firing radius of this enemy
	bool IsInShootingRadius(AActor* actor);
	// Checks if this actor is within a certain distance of another actor
	bool IsWithinDistance(AActor* fromActor, AActor* toActor, float distance);
};
