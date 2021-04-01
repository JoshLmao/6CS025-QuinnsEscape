// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleEscapeDoor.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
APuzzleEscapeDoor::APuzzleEscapeDoor()
{
	OpenAmount = FVector(-250.0f, 0, 0);	// Move to left
	Speed = 4.0f;

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
}

// Called when the game starts or when spawned
void APuzzleEscapeDoor::BeginPlay()
{
	Super::BeginPlay();

	// Store start location on begin
	m_startLocation = this->GetActorLocation();
	m_openedLocation = this->GetActorLocation() + OpenAmount;
}

// Called every frame
void APuzzleEscapeDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_bIsRaising)
	{
		// Increment lerp
		m_lerpValue += (Speed * DeltaTime);

		// Lerp from start to opened location
		FVector lerpLocation = FMath::Lerp(m_startLocation, m_openedLocation, m_lerpValue);
		this->SetActorLocation(lerpLocation);

		// Check if lerp is completed
		if (m_lerpValue >= 1.0f)
		{
			m_bIsRaised = true;
			m_bIsRaising = false;
		}
	}
}

void APuzzleEscapeDoor::OpenEscape()
{
	if (m_bIsRaised || m_bIsRaising)
	{
		return;
	}

	m_bIsRaising = true;
}
