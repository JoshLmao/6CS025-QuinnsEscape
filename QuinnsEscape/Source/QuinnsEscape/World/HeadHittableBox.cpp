// Fill out your copyright notice in the Description page of Project Settings.


#include "HeadHittableBox.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/TextRenderComponent.h"
#include "Engine/TextRenderActor.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "QuinnGameplayStatics.h"

// Sets default values
AHeadHittableBox::AHeadHittableBox()
{
	m_recievedHeadHitCount = 0;

	// Create mesh component and make root
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Component"));
	RootComponent = MeshComponent;
	// Block all mesh channels
	MeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

	// Load default cube, set if loaded
	static ConstructorHelpers::FObjectFinder<UStaticMesh> cubeMesh(TEXT("/Game/Geometry/Meshes/1M_Cube"));
	if (cubeMesh.Succeeded())
	{
		MeshComponent->SetStaticMesh(cubeMesh.Object);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to find 1M_Cube! Unable to set MeshComponent mesh"));
	}

	// Create box collider beneath box
	BeneathBoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Beneath Box Collider"));
	BeneathBoxCollider->SetupAttachment(RootComponent);
	BeneathBoxCollider->OnComponentBeginOverlap.AddDynamic(this, &AHeadHittableBox::OnOverlapBegin);
	// Add local offset & set extent (default values)
	BeneathBoxCollider->AddRelativeLocation(FVector(0, 0, -65.0f));
	BeneathBoxCollider->SetBoxExtent(FVector(55, 55, 20));

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
	AudioComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AHeadHittableBox::BeginPlay()
{
	Super::BeginPlay();

}

void AHeadHittableBox::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check that collided actor is 
	if (OtherActor->IsA(ACharacter::StaticClass()))
	{
		ACharacter* character = Cast<ACharacter>(OtherActor);
		// Detect if character is falling to classify a head hit
		if (character && character->GetMovementComponent() && character->GetMovementComponent()->IsFalling())
		{
			// Increment total head hit count
			m_recievedHeadHitCount++;

			// Play hit sound if valid
			if (IsValid(HitSound))
			{
				QuinnGameplayStatics::PlaySoundRndPitch(AudioComponent, HitSound, 0.9f, 1.1f);
			}

			// Trigger recieved head hit event
			if (OnRecievedHeadHit.IsBound())
			{
				OnRecievedHeadHit.Broadcast(character);
			}
		}
	}
}

ATextRenderActor* AHeadHittableBox::CreateBoxWithText(FString content, FColor color)
{
	// Spawn text actor in world
	ATextRenderActor* textActor = GetWorld()->SpawnActor<ATextRenderActor>(ATextRenderActor::StaticClass(), FVector(), FRotator());
	if (textActor)
	{
		// Snap TextComponent actor to Box
		FAttachmentTransformRules rules(EAttachmentRule::SnapToTarget, true);
		textActor->AttachToActor(this, rules);

		// Set text and color
		textActor->GetTextRender()->SetText(content);
		textActor->GetTextRender()->SetTextRenderColor(color);

		// Scale up to make visible
		textActor->SetActorScale3D(FVector(5.f, 5.f, 5.f));

		// Offset to infront of box
		textActor->SetActorRelativeLocation(FVector(BeneathBoxCollider->GetScaledBoxExtent().X, 30, -70));

		// Return created text
		return textActor;
	}

	// Error when creating text actor
	return nullptr;
}

void AHeadHittableBox::SetTextColor(ATextRenderActor* textActor, FColor color)
{
	if (IsValid(textActor))
	{
		textActor->GetTextRender()->SetTextRenderColor(color);
	}
}