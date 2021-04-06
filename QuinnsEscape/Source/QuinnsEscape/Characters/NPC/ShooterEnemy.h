

#pragma once

#include "CoreMinimal.h"
#include "Characters/HealthCharacter.h"
#include "ShooterEnemy.generated.h"

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
	// Is this shooter a static, non-moving shooting enemy
	UPROPERTY(EditAnywhere, Category = ShooterEnemyProperties)
	bool bIsStatic;
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

	float GetDetectionRadius();
	float GetShootingRadius();
	bool IsStaticEnemy();

	// Sets the targetting character of this enemy
	void SetTargetCharacter(ACharacter* target);

	// Gets the class type this enemy should target
	TSubclassOf<ACharacter> GetTargettingClass();

	// Lets the character to begin shooting at the set target character
	void BeginShooting();
	// Stops shooting at the target character
	void StopShooting();

protected:
	// Override
	virtual bool OnCharacterDeath() override;

	// Shoots at the current target character
	void ShootAtTarget();

	void LookAtTarget(FVector targetLocation);
};
