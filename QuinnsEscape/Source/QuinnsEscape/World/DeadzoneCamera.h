// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DeadzoneCamera.generated.h"

UCLASS()
class QUINNSESCAPE_API ADeadzoneCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADeadzoneCamera();

	/*
	*	VARIABLES
	*/
public:
	UPROPERTY(EditAnywhere)
	class AActor* ActorToFollow;

	// Field of view of camera
	UPROPERTY(EditAnywhere)
	float CameraFieldOfView;

	// Amount of offset between the character and the player
	UPROPERTY(EditAnywhere)
	float XOffset;
	// Amount to add to offset the height
	UPROPERTY(EditAnywhere)
	float ZOffset;

	// Amount of deadzone to use on the Y axis (Horizontal)
	UPROPERTY(EditAnywhere)
	float YDeadzone;
	// Amount of deadzone to use on the Z axis (height)
	UPROPERTY(EditAnywhere)
	float ZDeadzone;

	// Fixed height value to lock to instead of following the height
	UPROPERTY(EditAnywhere)
	float FixedZValue;

private:
	// Camera component of actor
	class UCameraComponent* m_camera;

	// Current Y position of camera
	float m_camY;
	// Current Z position of camera
	float m_camZ;
	// Value to lock the Z axis at
	float m_zAxisLock;

	/*
	*	METHODS
	*/
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Locks the Z axis (up/dpwn) of the camera to a certain value
	void LockZAxis(float zValue);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// Get the current camera Y positin if moved out of deadzone
	float GetCameraY();
	// Get the current camera Z position if moved out of deadzone
	float GetCameraZ();
};
