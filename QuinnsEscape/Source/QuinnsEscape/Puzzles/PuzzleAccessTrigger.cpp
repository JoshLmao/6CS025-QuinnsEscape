// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleAccessTrigger.h"
#include "Components/ShapeComponent.h"
#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"
#include "../Characters/QuinnCharacter.h"
#include "../World/DeadzoneCamera.h"

APuzzleAccessTrigger::APuzzleAccessTrigger()
{
	TeleportTarget = nullptr;
	CameraLockZ = 0;

	if (this->GetCollisionComponent() != nullptr)
	{
		this->GetCollisionComponent()->OnComponentBeginOverlap.AddDynamic(this, &APuzzleAccessTrigger::OnTriggerBeginOverlap);
	}
}

void APuzzleAccessTrigger::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// if other actor is main character
	if (OtherActor->IsA(AQuinnCharacter::StaticClass()))
	{
		// Teleport to location if target point has been set
		if (IsValid(TeleportTarget))
		{
			OtherActor->SetActorLocation(TeleportTarget->GetActorLocation(), false);

			// Get camera from level
			TArray<AActor*> actors;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADeadzoneCamera::StaticClass(), actors);
			if (actors.Num() > 0)
			{
				// Cast to deadzone and lock Z
				ADeadzoneCamera* camera = Cast<ADeadzoneCamera>(actors[0]);
				camera->LockZAxis(CameraLockZ);
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("No target point has been set on '%s'"), *this->GetName());
		}
	}
}
