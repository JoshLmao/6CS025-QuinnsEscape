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

	InputComponent->BindAction("Fire", IE_Pressed, this, &AQuinnPlayerController::OnFireReleased);

}

void AQuinnPlayerController::OnFireReleased()
{
	FVector actorLoc = GetPawn()->GetActorLocation();

	// Ray trace from mouse position to the world
	FHitResult result;
	GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, true, result);
	// Lock the raycast X to character's X
	result.Location.X = actorLoc.X;

	// Find look at from actors location to world position under mouse
	FRotator lookAtRotator = UKismetMathLibrary::FindLookAtRotation(actorLoc, result.Location);
	FVector fireDirectionVector = lookAtRotator.Vector();

	// Fire projectile towards to direction
	AQuinnCharacter* character = Cast<AQuinnCharacter>(GetPawn());
	character->FireProjectile(fireDirectionVector);
}