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

private:
	// Actor to use to fur
	UPROPERTY(EditAnywhere)
	class AActor* ProjectileActor;

	/*
	*	METHODS
	*/
public:
	/** Returns SideViewCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	// Called for side to side input
	void MoveRight(float Val);

private:
	// Fires a projectile
	void FireProjectile();
};
