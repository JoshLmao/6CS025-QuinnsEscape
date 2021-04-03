// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "../Characters/HealthCharacter.h"
#include "../Game/QuinnGameState.h"

// Sets default values
AProjectileBase::AProjectileBase()
{
	// Set default value of variables
	m_damage = 1.0f;

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup and configure root capsule component
	m_rootCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Root Collider"));
	RootComponent = m_rootCapsuleComponent;
	// Set to overlap all actors
	m_rootCapsuleComponent->SetCollisionProfileName("OverlapAllDynamic");

	// Configure root collider
	m_rootCapsuleComponent->SetCapsuleHalfHeight(30.0f);
	m_rootCapsuleComponent->SetCapsuleRadius(30.0f);
	//m_rootCapsuleComponent->bHiddenInGame = false; //display for debug purposes
	m_rootCapsuleComponent->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;
	// Listen to begin and end overlap events
	m_rootCapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AProjectileBase::OnColliderBeginOverlap);
	m_rootCapsuleComponent->OnComponentEndOverlap.AddDynamic(this, &AProjectileBase::OnColliderEndOverlap);

	// Add and configure projectile movement component
	m_projectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
	m_projectileMovementComponent->SetUpdatedComponent(m_rootCapsuleComponent);
	m_projectileMovementComponent->InitialSpeed = 30.0f;
	m_projectileMovementComponent->MaxSpeed = 3000.0f;
	m_projectileMovementComponent->bRotationFollowsVelocity = true;
	m_projectileMovementComponent->bShouldBounce = true;
	m_projectileMovementComponent->Bounciness = 0.3f;
	m_projectileMovementComponent->ProjectileGravityScale = 0.0f;
}

// Called when the game starts or when spawned
void AProjectileBase::BeginPlay()
{
	Super::BeginPlay();
	
	// Set a default life span
	SetLifeSpan(10.0f);
}

// Called every frame
void AProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AProjectileBase::GetSpeed()
{
	return m_projectileMovementComponent->InitialSpeed;
}

void AProjectileBase::SetSpeed(float speed)
{
	m_projectileMovementComponent->InitialSpeed = speed;
}

float AProjectileBase::GetDamage()
{
	return m_damage;
}
void AProjectileBase::SetDamage(float damage)
{
	m_damage = damage;
}

void AProjectileBase::FireInDirection(FVector fireDirection)
{
	m_projectileMovementComponent->Velocity = fireDirection * m_projectileMovementComponent->InitialSpeed;
}

void AProjectileBase::AddActorToIgnore(AActor* actor)
{
	m_ignoreActors.Add(actor);
}

void AProjectileBase::OnColliderBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check if other actor is of class that should be ignored
 	if (m_ignoreActors.Num() > 0)
	{
		for (AActor* actor : m_ignoreActors)
		{
			if (OtherActor == actor)
			{
				//UE_LOG(LogTemp, Log, TEXT("Projectile '%s' ignores actor '%s'"), *this->GetName(), *OtherActor->GetName());
				return;
			}
		}
	}

	// Ignore collision with self or other projectile
 	if (OtherActor->IsA(AProjectileBase::StaticClass()))
		return;

	// Collided with actor (Terrain/Character)
	if (OtherActor->IsA(AActor::StaticClass()))
	{
		// Check other actor is an character that contains health
		if (OtherActor->IsA(AHealthCharacter::StaticClass()))
		{
			AHealthCharacter* healthCharacter = Cast<AHealthCharacter>(OtherActor);
			// Actor is a HealthCharacter
			healthCharacter->TakeDamage(GetDamage(), FDamageEvent(), nullptr, this);

			UE_LOG(LogTemp, Log, TEXT("Projectile '%s' collided with Character '%s' and dealth '%f' damage (%f/%f)"), *this->GetName(), *healthCharacter->GetName(), GetDamage(), healthCharacter->GetCurrentHealth(), healthCharacter->GetTotalHealth());

			// Get game state and add score reward
			AGameStateBase* baseState = GetWorld()->GetGameState();
			if (IsValid(baseState))
			{
				AQuinnGameState* quinnState = Cast<AQuinnGameState>(baseState);
				quinnState->AddScore(healthCharacter->GetHitScoreReward());
			}
		}

		// Destroy projectile as it's collided with another actor
		this->Destroy();
	}
}

void AProjectileBase::OnColliderEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}
