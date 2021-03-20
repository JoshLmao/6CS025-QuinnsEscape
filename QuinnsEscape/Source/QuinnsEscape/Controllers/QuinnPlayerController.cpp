// Fill out your copyright notice in the Description page of Project Settings.


#include "QuinnPlayerController.h"
#include "GameFramework/Pawn.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Characters/QuinnCharacter.h"

AQuinnPlayerController::AQuinnPlayerController()
{
	// Use the mouse cursor for aiming
	bShowMouseCursor = true;
}

void AQuinnPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AQuinnPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
}

void AQuinnPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Input Actions
	// Fire
	InputComponent->BindAction("Fire", IE_Released, this, &AQuinnPlayerController::OnFireReleased);
	// Slam
	InputComponent->BindAction("Slam", IE_Released, this, &AQuinnPlayerController::OnSlamReleased);
	// Bind pressed and released of "Jump"
	InputComponent->BindAction("Jump", IE_Pressed, this, &AQuinnPlayerController::OnActionJump);
	InputComponent->BindAction("Jump", IE_Released, this, &AQuinnPlayerController::OnActionStopJumping);

	// Axis actions
	InputComponent->BindAxis("MoveRight", this, &AQuinnPlayerController::OnAxisMoveRight);
}

void AQuinnPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	QuinnCharacter = Cast<AQuinnCharacter>(aPawn);
}

void AQuinnPlayerController::OnUnPossess()
{
	Super::OnUnPossess();

	QuinnCharacter = nullptr;
}

void AQuinnPlayerController::OnFireReleased()
{
	if (IsLookInputIgnored())
	{
		return;
	}

	FVector actorLoc = GetPawn()->GetActorLocation();

	// Ray trace from mouse position to the world
	FHitResult result;
	GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, false, result);
	if (result.Location == FVector::ZeroVector)
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to determine mouse click. No object traced under location"));
	}
	else
	{
		// Lock the raycast X to character's X
		result.Location.X = actorLoc.X;

		// Find look at from actors location to world position under mouse
		FRotator lookAtRotator = UKismetMathLibrary::FindLookAtRotation(actorLoc, result.Location);
		FVector fireDirectionVector = lookAtRotator.Vector();

		// Fire projectile towards to direction
		QuinnCharacter->FireProjectile(fireDirectionVector);
	}
}

void AQuinnPlayerController::OnSlamReleased()
{
	QuinnCharacter->SlamGround();
}

void AQuinnPlayerController::OnAxisMoveRight(float value)
{
	// Check if move input should be ignored first
	if (IsMoveInputIgnored())
	{
		return;
	}

	QuinnCharacter->MoveRight(value);
}

void AQuinnPlayerController::OnActionJump()
{
	// Check if move input should be ignored first
	if (IsMoveInputIgnored())
	{
		return;
	}

	QuinnCharacter->Jump();
}

void AQuinnPlayerController::OnActionStopJumping()
{
	// Check if move input should be ignored first
	if (IsMoveInputIgnored())
	{
		return;
	}

	QuinnCharacter->StopJumping();
}
