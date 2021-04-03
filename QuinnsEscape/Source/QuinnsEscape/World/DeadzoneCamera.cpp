// Fill out your copyright notice in the Description page of Project Settings.


#include "DeadzoneCamera.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ADeadzoneCamera::ADeadzoneCamera()
{
	YDeadzone = 500;
	ZDeadzone = 200;
	CameraFieldOfView = 80.0f;
	m_camY = 0;
	m_camZ = 1000; // set at higher
	m_zAxisLock = 0;

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create camera component
	m_camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	m_camera->SetFieldOfView(CameraFieldOfView);
}

// Called when the game starts or when spawned
void ADeadzoneCamera::BeginPlay()
{
	Super::BeginPlay();
	
	// Set the main player camera to be this camera
	APlayerController* pc = UGameplayStatics::GetPlayerController(this, 0);
	pc->SetViewTarget(this, FViewTargetTransitionParams());

	// Set actor to follow
	ActorToFollow = pc->GetPawn();

	// Set field of view on start
	m_camera->SetFieldOfView(CameraFieldOfView);
}

// Called every frame
void ADeadzoneCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Set camera to follow if given an actor
	if (IsValid(ActorToFollow))
	{
		// Get Y and Z values
		m_camY = GetCameraY();
		m_camZ = GetCameraZ();
	}

	// Determine camera location
	FVector loc;
	if (m_zAxisLock != 0)
	{
		// Use Z axis locked value
		loc = FVector(XOffset, m_camY, m_zAxisLock + ZOffset);
	}
	else
	{
		// Update actor's position to target location when not locked
		loc = FVector(XOffset, m_camY, m_camZ + ZOffset);
	}

	// Set to location
	SetActorLocation(loc);
}

void ADeadzoneCamera::LockZAxis(float zValue)
{
	m_zAxisLock = zValue;
}

float ADeadzoneCamera::GetCameraY()
{
	FVector followLocation = ActorToFollow->GetActorLocation();
	
	// Check if actor to follow has moved left into deadzone
	float minus = followLocation.Y - YDeadzone;
	if (minus > GetActorLocation().Y)
	{
		return minus;
	}

	// Check if actor has moved right into deadzone
	float add = followLocation.Y + YDeadzone;
	if (add < GetActorLocation().Y)
	{
		return add;
	}

	// Return last cam X if player still within active zone
	return m_camY;
}

float ADeadzoneCamera::GetCameraZ()
{
	FVector followLocation = ActorToFollow->GetActorLocation();

	// Set to Z if a value is given
	if (FixedZValue != 0)
	{
		return FixedZValue;
	}

	// Update to move down if within deadzone
	float minus = followLocation.Z - ZDeadzone;
	if (minus > GetActorLocation().Z)
	{
		return minus;
	}

	// Update to move up if in deadzone
	float add = followLocation.Z + ZDeadzone;
	if (add < GetActorLocation().Z)
	{
		return add;
	}


	return m_camZ;
}
