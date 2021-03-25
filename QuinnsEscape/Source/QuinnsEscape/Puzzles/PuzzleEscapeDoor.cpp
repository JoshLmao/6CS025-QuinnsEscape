// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleEscapeDoor.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
APuzzleEscapeDoor::APuzzleEscapeDoor()
{
	RaiseAmount = FVector(0, 0, 250.0f);

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
}

// Called every frame
void APuzzleEscapeDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_bIsRaising)
	{
		// ToDo: lerp from starting position to 
		FVector raisedLoc = this->GetActorLocation() + RaiseAmount;
		this->SetActorLocation(raisedLoc);

		m_bIsRaised = true;
		m_bIsRaising = false;
	}
}


void APuzzleEscapeDoor::RaiseEscape()
{
	if (m_bIsRaised || m_bIsRaising)
	{
		return;
	}

	m_bIsRaising = true;
}
