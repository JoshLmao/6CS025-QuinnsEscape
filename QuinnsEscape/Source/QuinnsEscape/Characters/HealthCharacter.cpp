// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthCharacter.h"

// Sets default values
AHealthCharacter::AHealthCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set the total and current health to default values on start
	m_currentHealth = 0;
	m_totalHealth = 100;
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

	// Reduce health by damage amount 
	m_currentHealth -= damage;

	// If player has died, perform death checks
	if (m_currentHealth <= 0)
	{
		m_currentHealth = 0;

		// Call any overrides in parent class
		OnHealthCharacterDeath();

		// Broadcast event if anyone is listening
		if (OnCharacterDied.IsBound())
		{
			OnCharacterDied.Broadcast();
		}
	}

	return damage;
}

void AHealthCharacter::OnHealthCharacterDeath()
{
	// Empty, can be used by higher classes
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