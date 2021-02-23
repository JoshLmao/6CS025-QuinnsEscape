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
	UPROPERTY(EditAnywhere)
	float SlamTotalCooldown;

	// Speed that the projectile is fired at
	UPROPERTY(EditAnywhere)
	float ProjectileSpeed;
	// Amount of damage to deal to an enemy if the player lands on their head
	UPROPERTY(EditAnywhere)
	float HeadJumpDamage;
	// Amount multiplier to add to the HeadJumpDamage if the player slams onto an enemy head
	UPROPERTY(EditAnywhere)
	float SlamMultiplier;

	// How many lives the player starts with
	UPROPERTY(EditAnywhere)
	int StartLives;

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

protected:
	// BeginPlay function called on Actor begin
	virtual void BeginPlay() override;
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// Tick function called every frame
	virtual void Tick(float DeltaTime) override;
	// Jump function to force character in air
	virtual void Jump() override;

	virtual bool OnCharacterDeath() override;

	// Called for side to side input
	void MoveRight(float Val);

private:
	UFUNCTION()
	void OnStompCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
