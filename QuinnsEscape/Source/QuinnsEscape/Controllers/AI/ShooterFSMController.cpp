


#include "ShooterFSMController.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

#include "Characters/NPC/ShooterEnemy.h"

AShooterFSMController::AShooterFSMController()
{
	m_currentState = EShooterStates::Idle;
	m_fsmState = EFSMStates::Begin;
	m_possesedCharacter = nullptr;
}

void AShooterFSMController::BeginPlay()
{
	Super::BeginPlay();

}

void AShooterFSMController::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	// Run the finite state machine
	FSMUpdate(deltaTime);

	if (IsValid(m_possesedCharacter) && !IsValid(TargetCharacter))
	{
		// Get reference to target in scene
		TArray<AActor*> foundActors;
		TSubclassOf<ACharacter> targetChar = m_possesedCharacter->GetTargettingClass();
		const UWorld* world = this->GetWorld();
		UGameplayStatics::GetAllActorsOfClass(world, targetChar, foundActors);
		if (foundActors.Num() > 0)
		{
			// Use first found reference
			TargetCharacter = Cast<ACharacter>(foundActors[0]);
			UE_LOG(LogTemp, Log, TEXT("Controller '%s' targeting character '%s'"), *this->GetName(), *TargetCharacter->GetName());
			// Set target on character
			m_possesedCharacter->SetTargetCharacter(TargetCharacter);
		}
	}
}

void AShooterFSMController::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);

	if (pawn->IsA(AShooterEnemy::StaticClass()))
	{
		m_possesedCharacter = Cast<AShooterEnemy>(pawn);
		m_possesedCharacter->OnCharacterDied.AddDynamic(this, &AShooterFSMController::OnPossesedPawnDeath);
	}
}

void AShooterFSMController::OnUnPossess()
{
	Super::OnUnPossess();

	m_possesedCharacter = nullptr;
}

void AShooterFSMController::FSMUpdate(float deltaTime)
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


void AShooterFSMController::SetShooterState(EShooterStates newState)
{
	// Set current to new state
	m_currentState = newState;
	// Set FSM to run begin on new state
	m_fsmState = EFSMStates::Begin;
}

void AShooterFSMController::Idle_Begin()
{

}

void AShooterFSMController::Idle_Tick(float deltaTime)
{
	if (IsValid(m_possesedCharacter) && IsValid(TargetCharacter))
	{
		bool isStatic = m_possesedCharacter->IsStaticEnemy();
		// If not static, check for detection radius
		if (IsInDetectionRadius(TargetCharacter) && !isStatic)
		{
			SetShooterState(EShooterStates::Chasing);
		}
		// If static, check for in shooting range
		if (IsInShootingRadius(TargetCharacter) && isStatic)
		{
			SetShooterState(EShooterStates::Shooting);
		}
	}
}

void AShooterFSMController::Chasing_Begin()
{
	// Move to target's location
	EPathFollowingRequestResult::Type result = this->MoveToLocation(TargetCharacter->GetActorLocation());
	if (result != EPathFollowingRequestResult::Type::RequestSuccessful)
	{
		//UE_LOG(LogTemp, Error, TEXT("Chase Start: Request = %s"), result == EPathFollowingRequestResult::Type::Failed ? TEXT("FAILED") : TEXT("AT GOAL"));
	}
}

void AShooterFSMController::Chasing_Tick(float deltaTime)
{
	if (IsValid(TargetCharacter))
	{
		// Keep attempting to move to location with the previous request wasnt successul
		if (GetMoveStatus() != EPathFollowingRequestResult::Type::RequestSuccessful)
		{
			this->MoveToLocation(TargetCharacter->GetActorLocation());
		}

		// Check if target character within detection radius of this enemy
		if (IsInShootingRadius(TargetCharacter))
		{
			// Stop any move requests before transitioning state
			this->StopMovement();

			// In range, set to shooting
			SetShooterState(EShooterStates::Shooting);
		}
	}
}

void AShooterFSMController::Shooting_Begin()
{
	m_possesedCharacter->BeginShooting();
}

void AShooterFSMController::Shooting_Tick(float deltaTime)
{
	if (IsValid(m_possesedCharacter))
	{
		if (!IsInShootingRadius(TargetCharacter))
		{
			m_possesedCharacter->StopShooting();

			// If static, set to idle
			// Move to chasing if not static
			if (m_possesedCharacter->IsStaticEnemy())
				SetShooterState(EShooterStates::Idle);
			else
				SetShooterState(EShooterStates::Chasing);
		}
	}
}

void AShooterFSMController::Dead_Begin()
{
}

void AShooterFSMController::Dead_Tick(float deltaTime)
{
}

bool AShooterFSMController::IsInDetectionRadius(AActor* actor)
{
	return IsWithinDistance(m_possesedCharacter, actor, m_possesedCharacter->GetDetectionRadius());
}

bool AShooterFSMController::IsInShootingRadius(AActor* actor)
{
	return IsWithinDistance(m_possesedCharacter, actor, m_possesedCharacter->GetShootingRadius());
}

bool AShooterFSMController::IsWithinDistance(AActor* fromActor, AActor* toActor, float distance)
{
	float dist = FVector::Dist(fromActor->GetActorLocation(), toActor->GetActorLocation());
	return dist <= distance;
}

void AShooterFSMController::OnPossesedPawnDeath()
{
	// When character dies, set FSM state to dead
	SetShooterState(EShooterStates::Dead);
}