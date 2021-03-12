// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerupBox.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "../Characters/QuinnCharacter.h"
#include "PowerupPickup.h"

// Sets default values
APowerupBox::APowerupBox()
{
	// Default values for variables
	PowerupSpawnLocation = FVector(0, 0, 100.0f);
	MaxPowerupSpawnCount = 1;

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create mesh component and make root
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	RootComponent = MeshComponent;
	// Ignore all collision on all channels
	MeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	// Create box collider beneath box
	BeneathBoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Beneath Box Collider"));
	BeneathBoxCollider->SetupAttachment(RootComponent);
	BeneathBoxCollider->OnComponentBeginOverlap.AddDynamic(this, &APowerupBox::OnOverlapBegin);
	// Add local offset & set extent
	BeneathBoxCollider->AddRelativeLocation(FVector(0, 0, -75.0f));
	BeneathBoxCollider->SetBoxExtent(FVector(65, 65, 30));
}

// Called when the game starts or when spawned
void APowerupBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APowerupBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APowerupBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check that collided actor is quinn
	if (OtherActor->IsA(AQuinnCharacter::StaticClass()) && CanSpawnPowerup())
	{
		UE_LOG(LogTemp, Log, TEXT("Quinn collided with power-up box '%s'"), *GetName());

		// Spawn random powerup
		bool success = SpawnRandomPowerup();
		if (success)
		{
			// Increment spawn count
			m_spawnedPowerupCount++;
		}
	}
}

bool APowerupBox::SpawnRandomPowerup()
{
	// Check powerups array contains values
	if (AllPowerups.Num() <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't spawn powerup. No powerups have been set"));
		return false;
	}

	// Get random index inside range
	int rndIndex = FMath::RandRange(0, AllPowerups.Num() - 1);
	// Spawn that index above box
	return SpawnPowerup(AllPowerups[rndIndex]);
}

bool APowerupBox::SpawnPowerup(TSubclassOf<APowerupPickup> powerup)
{
	// Check powerup is valid
	if (!IsValid(powerup))
	{
		UE_LOG(LogTemp, Error, TEXT("No valid powerup passed to SpawnPowerup!"));
		return false;
	}

	// Add relative spawn location for powerup
	FVector spawnLoc = GetActorLocation() + PowerupSpawnLocation;
	// Spawn powerup actor
	APowerupPickup* spawnedPowerup = GetWorld()->SpawnActor<APowerupPickup>(powerup, spawnLoc, FRotator());
	if (IsValid(spawnedPowerup))
	{
		UE_LOG(LogTemp, Log, TEXT("Spawned '%s' powerup"), *powerup->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Error when spawning '%s' powerup"), *powerup->GetName());
		return false;
	}

	return true;
}

bool APowerupBox::CanSpawnPowerup()
{
	return m_spawnedPowerupCount < MaxPowerupSpawnCount;
}

