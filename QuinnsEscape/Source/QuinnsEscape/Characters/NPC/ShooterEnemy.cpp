


#include "ShooterEnemy.h"
#include "TimerManager.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"

#include "Projectiles/ProjectileBase.h"
#include "Controllers/AI/ShooterFSMController.h"

AShooterEnemy::AShooterEnemy()
{
	// Set default values
	DetectionRadius = 750.0f;
	ShootingRadius = 500.0f;
	ShootingInteval = 1.0f;		// One second
	ProjectileSpeed = 1.0f;
	ProjectileDamage = FFloatRange(10.0f, 15.0f);
	bIsStatic = false;

	TargetClass = AHealthCharacter::StaticClass();
	ProjectileClass = AProjectileBase::StaticClass();
	
	// Set to be controlled by specific Shooter FSM
	AIControllerClass = AShooterFSMController::StaticClass();
	// Set place or spawn to possessed by AI Controller
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCharacterMovement()->bCanWalkOffLedges = true;
	GetCharacterMovement()->MaxWalkSpeed = 550.0f;	// 50 more than player
}

void AShooterEnemy::BeginPlay()
{
	Super::BeginPlay();

	// Get controller as AI Controller
	AIController = Cast<AAIController>(GetController());
	if (!IsValid(AIController))
	{
		UE_LOG(LogTemp, Error, TEXT("No reference to AIController in '%s'!"), *this->GetName());
	}
}

void AShooterEnemy::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

}

bool AShooterEnemy::OnCharacterDeath()
{
	bool isDead = Super::OnCharacterDeath();

	// Check if fire timer is active and stop
	if (GetWorldTimerManager().IsTimerActive(m_fireDelayHandle))
	{
		GetWorldTimerManager().ClearTimer(m_fireDelayHandle);
	}

	return isDead;
}

float AShooterEnemy::GetDetectionRadius()
{
	return DetectionRadius;
}

float AShooterEnemy::GetShootingRadius()
{
	return ShootingRadius;
}

bool AShooterEnemy::IsStaticEnemy()
{
	return bIsStatic;
}

void AShooterEnemy::SetTargetCharacter(ACharacter* target)
{
	TargetCharacter = target;
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
		// Set look at target when firing
		this->LookAtTarget(TargetCharacter->GetActorLocation());
		
		// Determine direction projectile should fly to get from current location to target character location
		FVector targetLocation = TargetCharacter->GetActorLocation();
		FVector projectileDirection = targetLocation - this->GetActorLocation();
	
		// Spawn projectile initially, check it spawned okay
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

void AShooterEnemy::LookAtTarget(FVector targetLocation)
{
	// Set Actor to look at target actor
	FRotator lookAt = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), targetLocation);
	FRotator actorRotation = this->GetActorRotation();
	actorRotation.Yaw = lookAt.Yaw;
	actorRotation.Roll = actorRotation.Pitch = 0.0f;	// Enforce other axis' to 0

	this->SetActorRotation(actorRotation);

	// Also set Controller's look rotation
	GetController()->SetControlRotation(actorRotation);
}

TSubclassOf<ACharacter> AShooterEnemy::GetTargettingClass()
{
	return TargetClass;
}

void AShooterEnemy::BeginShooting()
{
	// Check timer isn't already active. If it is, clear it
	if (!GetWorldTimerManager().IsTimerActive(m_fireDelayHandle))
	{
		GetWorldTimerManager().ClearTimer(m_fireDelayHandle);
	}

	// Start timer normally. Set to loop and have initial delay of 0 to fire right away
	GetWorldTimerManager().SetTimer(m_fireDelayHandle, this, &AShooterEnemy::ShootAtTarget, ShootingInteval, true, 0.0f);
}

void AShooterEnemy::StopShooting()
{
	// Clear timer to stop shooting
	GetWorldTimerManager().ClearTimer(m_fireDelayHandle);
}
