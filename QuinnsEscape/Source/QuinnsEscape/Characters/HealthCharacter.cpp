// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AHealthCharacter::AHealthCharacter()
{
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
	DetachFromControllerPendingDestroy();

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
