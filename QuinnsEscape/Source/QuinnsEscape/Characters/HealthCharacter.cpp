// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "../Game/QuinnGameState.h"

// Sets default values
AHealthCharacter::AHealthCharacter()
{
	HitScore = 0.5;
	KilledScore = 5;
	m_aliveTimeSecs = 0;

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set the total and current health to default values on start
	m_currentHealth = 0;
	m_totalHealth = 100;

	//Enable overlap events on capsule
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	// Set charatcer to recieve stomp
	this->SetCanRecieveStomp(true);
}

// Called when the game starts or when spawned
void AHealthCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// Set the current health to the total on start
	SetCurrentHealth(GetTotalHealth());
}

// Called every frame
void AHealthCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Increment alive time if character is alive
	if (this->GetIsCharacterAlive())
	{
		m_aliveTimeSecs += DeltaTime;
	}
}

// Called to bind functionality to input
void AHealthCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Called when character takes damage
float AHealthCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	
	// Deal amount to player
	DealDamage(damage);

	return damage;
}

void AHealthCharacter::FellOutOfWorld(const UDamageType& dmgType)
{
	// Deal massive damage if falling out of world
	DealDamage(99999.0f);

	// Only call super to destroy if should be destroyed
	if (m_currentHealth <= 0 && GetDestroyOnFall())
	{
		Super::FellOutOfWorld(dmgType);
	}
}

FVector AHealthCharacter::GetHalfLocationWithOffset(float yOffset)
{
	// Get half height and radius of capsule
	float halfHeight, radius;
	GetCapsuleComponent()->GetScaledCapsuleSize(radius, halfHeight);

	// Determine location to be in the middle of character
	FVector btmCenterLocation = GetActorLocation();
	// Determine location half way up character
	FVector upAxisHalfHeightOffset = GetActorUpVector() * (halfHeight / 2);
	// Add center offset to btm center
	FVector halfLocation = btmCenterLocation + upAxisHalfHeightOffset;
	
	// Set X to same X as actor
	halfLocation.X = this->GetActorLocation().X;
	
	// Add the given offset
	halfLocation.Y += yOffset;

	// Return final spawn locatino
	return halfLocation;
}

void AHealthCharacter::DealDamage(float damage)
{
	// Reduce health by damage amount 
	m_currentHealth -= damage;

	// If player has died, perform death checks
	if (m_currentHealth <= 0)
	{
		m_currentHealth = 0;

		// Call any overrides in parent class
		bool isDead = OnCharacterDeath();

		UE_LOG(LogTemp, Log, TEXT("%s IsDead = %d"), *GetName(), isDead ? 1 : 0);
		// If character should die, continue death
		if (isDead)
		{
			// Broadcast event if anyone is listening
			if (OnCharacterDied.IsBound())
			{
				OnCharacterDied.Broadcast();
			}

			// Reward with killed score
			AGameStateBase* state = GetWorld()->GetGameState();
			if (IsValid(state))
			{
				AQuinnGameState* quinnState = Cast<AQuinnGameState>(state);
				quinnState->AddScore(this->GetKilledScoreReward());
			}
		}
	}
}

bool AHealthCharacter::TakeStomp(float damage)
{
	// Check if this character can be stomped
	if (this->CanRecieveStomp())
	{
		// Deal the damage
		float dmg = this->TakeDamage(damage, FDamageEvent(), nullptr, nullptr);
		return dmg > 0;
	}

	return false;
}

bool AHealthCharacter::OnCharacterDeath()
{
	// Ragdoll character on it's death

	// Disable any collision on main capsule
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);

	SetActorEnableCollision(true);
	// Configure mesh to use ragdoll
	GetMesh()->SetCollisionProfileName("Ragdoll");
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->WakeAllRigidBodies();
	GetMesh()->bBlendPhysics = true;
	
	// Configure movement component to stop and prevent movement
	if (UPawnMovementComponent* mc = GetMovementComponent())
	{
		mc->StopMovementImmediately();
		mc->SetComponentTickEnabled(false);

		// Cast to disable movement on character
		if (UCharacterMovementComponent* cmc = Cast<UCharacterMovementComponent>(mc))
		{
			cmc->DisableMovement();
		}
	}

	// Disappear character after delay
	float timeoutSpan = 10.0f;
	SetLifeSpan(timeoutSpan);

	UE_LOG(LogTemp, Log, TEXT("Character '%s' has died. Lifespan of '%f' seconds"), *GetName(), timeoutSpan);

	return true; // Kill character on death
}

double AHealthCharacter::GetHitScoreReward()
{
	return HitScore;
}

void AHealthCharacter::SetHitScoreReward(double score)
{
	HitScore = score;
}

double AHealthCharacter::GetKilledScoreReward()
{
	return KilledScore;
}

void AHealthCharacter::SetKilledScoreReward(double score)
{
	KilledScore = score;
}

float AHealthCharacter::GetCurrentHealth()
{
	return m_currentHealth;
}
void AHealthCharacter::SetCurrentHealth(float health)
{
	m_currentHealth = FMath::Clamp<float>(health, 0, m_totalHealth);
}

float AHealthCharacter::GetTotalHealth()
{
	return m_totalHealth;
}
void AHealthCharacter::SetTotalHealth(float health)
{
	m_totalHealth = health;
}

bool AHealthCharacter::CanRecieveStomp()
{
	return m_canRecieveStomp;
}

void AHealthCharacter::SetCanRecieveStomp(bool canBeStomped)
{
	m_canRecieveStomp = canBeStomped;
}

bool AHealthCharacter::GetDestroyOnFall()
{
	return m_shouldDestroyOnFall;
}

// Set if the character should be destroyed when falling out of level
void AHealthCharacter::SetDestroyOnFall(bool shouldDestroy)
{
	m_shouldDestroyOnFall = shouldDestroy;
}

bool AHealthCharacter::GetIsCharacterAlive()
{
	return this->GetCurrentHealth() > 0;
}

float AHealthCharacter::GetCharacterAliveDuration()
{
	return m_aliveTimeSecs;
}
