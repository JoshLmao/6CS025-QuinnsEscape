


#include "ShooterEnemy.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "Projectiles/ProjectileBase.h"

AShooterEnemy::AShooterEnemy()
{
	// Set default values
	DetectionRadius = 750.0f;
	ShootingRadius = 500.0f;
	ShootingInteval = 1.0f;		// One second
	ProjectileSpeed = 1.0f;
	ProjectileDamage = FFloatRange(10.0f, 15.0f);
	bIsStatic = false;

	m_currentState = EShooterStates::Idle;
	m_fsmState = EFSMStates::Begin;

	TargetClass = AHealthCharacter::StaticClass();
	ProjectileClass = AProjectileBase::StaticClass();

	// Set to be controlled by AI
	AIControllerClass = AAIController::StaticClass();

	GetCharacterMovement()->bCanWalkOffLedges = true;
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

	// Get controller as AI Controller
	AIController = Cast<AAIController>(GetController());
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
	if (GetWorldTimerManager().IsTimerActive(m_fireDelayHandle))
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
		// If not static, check for detection radius
		if (IsInDetectionRadius(TargetCharacter) && !bIsStatic)
		{
			SetShooterState(EShooterStates::Chasing);
		}
		// If static, check for in shooting range
		if (IsInShootingRadius(TargetCharacter) && bIsStatic)
		{
			SetShooterState(EShooterStates::Shooting);
		}
	}
}

void AShooterEnemy::Chasing_Begin()
{
	// Move to target's location
	if (IsValid(AIController))
	{
		EPathFollowingRequestResult::Type result = AIController->MoveToLocation(TargetCharacter->GetActorLocation());
		if (result != EPathFollowingRequestResult::Type::RequestSuccessful)
		{
			//UE_LOG(LogTemp, Error, TEXT("Chase Start: Request = %s"), result == EPathFollowingRequestResult::Type::Failed ? TEXT("FAILED") : TEXT("AT GOAL"));
		}
	}
}

void AShooterEnemy::Chasing_Tick(float deltaTime)
{
	if (IsValid(TargetCharacter))
	{
		// Keep attempting to move to location with the previous request wasnt successul
		if (AIController->GetMoveStatus() != EPathFollowingRequestResult::Type::RequestSuccessful)
		{
			AIController->MoveToLocation(TargetCharacter->GetActorLocation());
		}

		// Check if target character within detection radius of this enemy
		if (IsInShootingRadius(TargetCharacter))
		{
			// Stop any move requests before transitioning state
			AIController->StopMovement();

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
	if (IsValid(TargetCharacter))
	{
		if (!IsInShootingRadius(TargetCharacter))
		{
			GetWorldTimerManager().ClearTimer(m_fireDelayHandle);

			// If static, set to idle
			// Move to chasing if not static
			if (bIsStatic)
				SetShooterState(EShooterStates::Idle);
			else
				SetShooterState(EShooterStates::Chasing);
		}
	}
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

bool AShooterEnemy::IsInDetectionRadius(AActor* actor)
{
	return IsWithinDistance(actor, DetectionRadius);
}

void AShooterEnemy::LookAtTarget(FVector targetLocation)
{
	// Set Actor to look at target actor
	FRotator lookAt = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), targetLocation);
	FRotator actorRotation = this->GetActorRotation();
	actorRotation.Yaw = lookAt.Yaw + -90.0f;			// Add Yaw offset
	actorRotation.Roll = actorRotation.Pitch = 0.0f;	// Enforce other axis' to 0

	this->SetActorRotation(actorRotation);

	// Also set Controller's look rotation
	GetController()->SetControlRotation(actorRotation);
}

bool AShooterEnemy::IsInShootingRadius(AActor* actor)
{
	return IsWithinDistance(actor, ShootingRadius);
}

bool AShooterEnemy::IsWithinDistance(AActor* actor, float distance)
{
	float dist = FVector::Dist(this->GetActorLocation(), actor->GetActorLocation());
	return dist <= distance;
}