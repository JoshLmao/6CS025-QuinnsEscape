// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerupPickup.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Components/BoxComponent.h"
#include "../Characters/QuinnCharacter.h"
#include "Engine/World.h"
#include "../Game/QuinnGameState.h"

// Sets default values
APowerupPickup::APowerupPickup()
{
	RotationSpeed = 25.0f;
	FloatSpeed = 5.0f;
	ZMoveAmount = 10.0f;
	m_animGoUp = true;
	m_minZ = m_maxZ = 0;

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create and set static mesh as root
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh Component"));
	RootComponent = StaticMeshComponent;
	// No collision on powerup mesh representation
	StaticMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));
	// Set Mesh if set
	if (IsValid(Mesh))
		StaticMeshComponent->SetStaticMesh(Mesh);

	// Create box trigger
	BoxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Trigger"));
	BoxTrigger->SetupAttachment(RootComponent);
	BoxTrigger->OnComponentBeginOverlap.AddDynamic(this, &APowerupPickup::OnActorBeginOverlap);
	// Set trigger size
	BoxTrigger->SetBoxExtent(FVector(75, 75, 75));
}

// Called when the game starts or when spawned
void APowerupPickup::BeginPlay()
{
	Super::BeginPlay();
	
	// Set min and max Z values to float between for animation
	m_minZ = this->GetActorLocation().Z - (ZMoveAmount / 2);
	m_maxZ = this->GetActorLocation().Z + (ZMoveAmount / 2);
}

// Called every frame
void APowerupPickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Perform spin anim
	DoSpinAnimation(DeltaTime);
}

void APowerupPickup::OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Detect Quinn character picks up ability
	if (OtherActor->IsA(AQuinnCharacter::StaticClass()))
	{
		UE_LOG(LogTemp, Log, TEXT("Quinn overlapped with '%s'"), *this->GetName());

		// Apply effects of powerup
		APawn* pawn = Cast<APawn>(OtherActor);
		ApplyEffects(pawn);

		// Destroy pickup once complete
		this->Destroy();
	}
}

void APowerupPickup::ApplyEffects(APawn* playerPawn)
{
	// Can be overrided. Applies the relevant effects of the pickup
}

AQuinnGameState* APowerupPickup::GetQuinnGameState()
{
	// Get base state and cast
	AGameStateBase* baseState = GetWorld()->GetGameState();
	if (AQuinnGameState* gameState = Cast<AQuinnGameState>(baseState))
	{
		// Return cast if successful
		return gameState;
	}
	// Return nullptr since we couldnt cast
	return nullptr;
}

void APowerupPickup::DoSpinAnimation(float deltaTime)
{
	// Add spin
	FRotator lastRotator = this->GetActorRotation();
	// Spin correct direction
	if (bRotateLeft)
		lastRotator = lastRotator + (FRotator(0, RotationSpeed * deltaTime, 0));
	else
		lastRotator = lastRotator - (FRotator(0, RotationSpeed * deltaTime, 0));

	SetActorRotation(lastRotator);

	// Bob up and down
	FVector lastLocation = this->GetActorLocation();
	if (m_animGoUp)
	{
		lastLocation.Z = lastLocation.Z + (FloatSpeed * deltaTime);
		if (lastLocation.Z >= m_maxZ)
		{
			m_animGoUp = false;
		}
	}
	else
	{
		lastLocation.Z = lastLocation.Z - (FloatSpeed * deltaTime);
		if (lastLocation.Z <= m_minZ)
		{
			m_animGoUp = true;
		}
	}
	this->SetActorLocation(lastLocation);
}