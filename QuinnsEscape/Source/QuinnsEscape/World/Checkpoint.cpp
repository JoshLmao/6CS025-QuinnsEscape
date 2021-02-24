// Fill out your copyright notice in the Description page of Project Settings.


#include "Checkpoint.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

#include "../Characters/QuinnCharacter.h"

// Sets default values
ACheckpoint::ACheckpoint()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	m_hasPassed = false;
	BoxExtent = FVector(100.0f, 50.0f, 100.0f);
	m_lerpAlpha = -20.0f;

	// Create Box Trigger component and set as root component
	m_pointTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Checkpoint Trigger"));
	RootComponent = m_pointTrigger;

	// Configure box trigger
	m_pointTrigger->SetBoxExtent(BoxExtent);
	m_pointTrigger->OnComponentBeginOverlap.AddDynamic(this, &ACheckpoint::CheckpointBeginOverlap);
	m_pointTrigger->bHiddenInGame = false;

	// Create mesh component and attack to root
	m_staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	m_staticMesh->SetupAttachment(RootComponent);
	// Ignore any/all collision on mesh
	m_staticMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
}

// Called when the game starts or when spawned
void ACheckpoint::BeginPlay()
{
	Super::BeginPlay();
	
	// Add checkpoint mesh model if exists
	if (CheckpointMesh)
	{
		m_staticMesh->SetStaticMesh(CheckpointMesh);
		m_staticMesh->SetRelativeLocation(FVector(85, 0, -100));
		m_staticMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, -90.0f));
	}
}

void ACheckpoint::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	// Check lerp alpha is within range to execute
	if (m_lerpAlpha >= 0 && m_lerpAlpha < 1.2)
	{
		// Lerp between start and end value to rotate mesh upright
		// m_lerpAlpha should range between 0 and 1. 
		// Allow some overflow to make sure roll == end
		float start = -90.0f;
		float end = 0.0f;
		float roll = FMath::Lerp(start, end, m_lerpAlpha);
		
		// Increment alpha with the delta time and speed
		float speed = 3.0f;
		m_lerpAlpha += deltaTime * speed;

		// If overflow, set to target end value
		if (roll > end)
			roll = end;

		// Set final rotation to mesh
		m_staticMesh->SetRelativeRotation(FRotator(0.0, 0.0, roll));
	}
}

void ACheckpoint::CheckpointBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check other actor is Quinn & hasn't already passed
	if (OtherActor->IsA(AQuinnCharacter::StaticClass()) && !m_hasPassed)
	{
		// Set passed flag to only set this checkpoint once
		m_hasPassed = true;

		// Cast actor to Quinn character and set this as latest checkpoint
		AQuinnCharacter* quinn = Cast<AQuinnCharacter>(OtherActor);
		if (IsValid(quinn))
		{
			quinn->SetCheckpoint(this);
			UE_LOG(LogTemp, Log, TEXT("Player passed through '%s' checkpoint! Set as latest"), *GetName());

			m_lerpAlpha = 0.0f;
		}
	}
}
