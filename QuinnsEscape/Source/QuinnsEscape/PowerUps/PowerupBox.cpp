// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerupBox.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "../Characters/QuinnCharacter.h"
#include "PowerupPickup.h"
#include "Runtime/Engine/Classes/Engine/TextRenderActor.h"
#include "Engine/TextRenderActor.h"
#include "Components/TextRenderComponent.h"


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
	// Block all mesh channels
	MeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	// Create box collider beneath box
	BeneathBoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Beneath Box Collider"));
	BeneathBoxCollider->SetupAttachment(RootComponent);
	BeneathBoxCollider->OnComponentBeginOverlap.AddDynamic(this, &APowerupBox::OnOverlapBegin);
	// Add local offset & set extent (default values)
	BeneathBoxCollider->AddRelativeLocation(FVector(0, 0, -65.0f));
	BeneathBoxCollider->SetBoxExtent(FVector(55, 55, 20));
}

// Called when the game starts or when spawned
void APowerupBox::BeginPlay()
{
	Super::BeginPlay();
	
	CreateTextComponent();
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

		// Change text color once power up spawned
		if (TextComponent)
			TextComponent->GetTextRender()->SetTextRenderColor(FColor::Yellow);
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

void APowerupBox::CreateTextComponent()
{
	if (TextComponent)
	{
		// Already created
		return;
	}

	// Spawn component in world
	TextComponent = GetWorld()->SpawnActor<ATextRenderActor>(ATextRenderActor::StaticClass(), FVector(), FRotator());
	if (TextComponent)
	{
		// Snap TextComponent actor to Box
		FAttachmentTransformRules rules(EAttachmentRule::SnapToTarget, true);
		TextComponent->AttachToActor(this, rules);

		// Set text and color
		TextComponent->GetTextRender()->SetText(FText::FromString(TEXT("#")));
		TextComponent->GetTextRender()->SetTextRenderColor(FColor::Black);
		// Scale up to make visible
		TextComponent->SetActorScale3D(FVector(5.f, 5.f, 5.f));
		// Offset to infront of box
		TextComponent->SetActorRelativeLocation(FVector(BeneathBoxCollider->GetScaledBoxExtent().X, 30, -70));
	}
}