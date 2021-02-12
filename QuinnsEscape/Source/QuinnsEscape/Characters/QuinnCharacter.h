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

private:
	// Actor to use to fur
	UPROPERTY(EditAnywhere)
	class AActor* ProjectileActor;

	// Current cooldown of firing a projectile
	float m_fireCooldown;

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

protected:
	// BeginPlay function called on Actor begin
	virtual void BeginPlay() override;
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	// Tick function called every frame
	virtual void Tick(float DeltaTime) override;

	// Called for side to side input
	void MoveRight(float Val);
};
