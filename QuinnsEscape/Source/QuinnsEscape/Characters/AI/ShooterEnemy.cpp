


#include "ShooterEnemy.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

#include "Projectiles/ProjectileBase.h"

AShooterEnemy::AShooterEnemy()
{
	// Set default values
	DetectionRadius = 750.0f;
	ShootingRadius = 400.0f;
	ShootingInteval = 1.0f;		// One second
	ProjectileSpeed = 1.0f;
	ProjectileDamage = FFloatRange(10.0f, 15.0f);

	m_currentState = EShooterStates::Idle;
	m_fsmState = EFSMStates::Begin;

	TargetClass = AHealthCharacter::StaticClass();
	ProjectileClass = AProjectileBase::StaticClass();
}

void AShooterEnemy::BeginPlay()
{
	Super::BeginPlay();

	// Get reference to target on start
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), TargetClass, foundActors);
	if (foundActors.Num() > 0)
	{
		// Use first found reference
		TargetCharacter = Cast<ACharacter>(foundActors[0]);
		UE_LOG(LogTemp, Log, TEXT("NPC '%s' targeting character '%s'"), *this->GetName(), *TargetCharacter->GetName());
	}
}

void AShooterEnemy::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	// Run the finite state machine
	FSMUpdate(deltaTime);
}

bool AShooterEnemy::OnCharacterDeath()
{
	bool isDead = Super::OnCharacterDeath();

	// Check if fire timer is active and stop
	if (GetWorldTimerManager().IsTimerPending(m_fireDelayHandle))
	{
		GetWorldTimerManager().ClearTimer(m_fireDelayHandle);
	}

	// Set shooter state to death
	SetShooterState(EShooterStates::Dead);

	return isDead;
}

void AShooterEnemy::FSMUpdate(float deltaTime)
{
	// Switch through all potential states of the enemy
	switch (m_currentState)
	{
		case EShooterStates::Idle:
		{
			// Determine FSM State and run relevant code
			if (m_fsmState == EFSMStates::Begin) {
				Idle_Begin();
				// Set state to Tick after begin run once
				m_fsmState = EFSMStates::Tick;
			}
			else if (m_fsmState == EFSMStates::Tick) {
				Idle_Tick(deltaTime);
			}
			break;
		}
		case EShooterStates::Chasing:
		{
			if (m_fsmState == EFSMStates::Begin) {
				Chasing_Begin();
				m_fsmState = EFSMStates::Tick;
			}
			else if (m_fsmState == EFSMStates::Tick) {
				Chasing_Tick(deltaTime);
			}
			break;
		}
		case EShooterStates::Shooting:
		{
			if (m_fsmState == EFSMStates::Begin) {
				Shooting_Begin();
				m_fsmState = EFSMStates::Tick;
			}
			else if (m_fsmState == EFSMStates::Tick) {
				Shooting_Tick(deltaTime);
			}
			break;
		}
		case EShooterStates::Dead:
		{
			if (m_fsmState == EFSMStates::Begin) {
				Dead_Begin();
				m_fsmState = EFSMStates::Tick;
			}
			else if (m_fsmState == EFSMStates::Tick) {
				Dead_Tick(deltaTime);
			}
			break;
		}
		default:
		{
			// Check for any unexpected state
			UE_LOG(LogTemp, Error, TEXT("Unexpected/Unimplemented EShooterState in enemy '%s'!"), *this->GetName());
			break;
		}
	}
}

void AShooterEnemy::SetShooterState(EShooterStates newState)
{
	// Set current to new state
	m_currentState = newState;
	// Set FSM to run begin on new state
	m_fsmState = EFSMStates::Begin;
}

void AShooterEnemy::Idle_Begin()
{
}

void AShooterEnemy::Idle_Tick(float deltaTime)
{
	if (IsValid(TargetCharacter))
	{
		// Is target character within detection radius of this enemy
		float dist = FVector::Dist(this->GetActorLocation(), TargetCharacter->GetActorLocation());
		if (dist <= DetectionRadius)
		{
			SetShooterState(EShooterStates::Chasing);
		}
	}
}

void AShooterEnemy::Chasing_Begin()
{
}

void AShooterEnemy::Chasing_Tick(float deltaTime)
{
	if (IsValid(TargetCharacter))
	{
		// Is target character within detection radius of this enemy
		float dist = FVector::Dist(this->GetActorLocation(), TargetCharacter->GetActorLocation());
		if (dist <= ShootingRadius)
		{
			SetShooterState(EShooterStates::Shooting);
		}
	}
}

void AShooterEnemy::Shooting_Begin()
{
	// Check timer isn't already active. If it is, clear it
	if (!GetWorldTimerManager().IsTimerActive(m_fireDelayHandle))
	{
		GetWorldTimerManager().ClearTimer(m_fireDelayHandle);
	}

	// Start timer normally
	GetWorldTimerManager().SetTimer(m_fireDelayHandle, this, &AShooterEnemy::ShootAtTarget, ShootingInteval, true);
}

void AShooterEnemy::Shooting_Tick(float deltaTime)
{
}

void AShooterEnemy::Dead_Begin()
{
}

void AShooterEnemy::Dead_Tick(float deltaTime)
{
}

void AShooterEnemy::ShootAtTarget()
{
	if (!IsValid(ProjectileClass))
	{
		UE_LOG(LogTemp, Error, TEXT("No projectile class set!"));
		return;
	}
	 
	if (IsValid(TargetCharacter))
	{
		// Determine direction projectile should fly to get from current location to target character location
		FVector targetLocation = TargetCharacter->GetActorLocation();
		FVector projectileDirection = targetLocation - this->GetActorLocation();
	
		// Determine spawning location

		AActor* projectileActor = GetWorld()->SpawnActor<AActor>(ProjectileClass->GetDefaultObject()->GetClass());
		if (IsValid(projectileActor))
		{
			// Cast and configure projectile to properties
			AProjectileBase* projectile = Cast<AProjectileBase>(projectileActor);
			projectile->AddActorToIgnore(this);		// ignore self
			projectile->SetSpeed(ProjectileSpeed);

			// Get random damage within bounds and set
			float rndDamage = FMath::RandRange(ProjectileDamage.GetLowerBoundValue(), ProjectileDamage.GetUpperBoundValue());
			projectile->SetDamage(rndDamage);
			// Set firing direction
			projectile->FireInDirection(projectileDirection);

			// Set spawn location to half height of character
			FVector halfLocation = this->GetHalfLocationWithOffset(0.0f);
			projectile->SetActorLocation(halfLocation);
		}
	}
}