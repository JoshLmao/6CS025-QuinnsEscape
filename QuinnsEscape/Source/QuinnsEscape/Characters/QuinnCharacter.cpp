// Fill out your copyright notice in the Description page of Project Settings.


#include "QuinnCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "Sound/SoundBase.h"

#include "../Projectiles/ProjectileBase.h"
#include "../World/Checkpoint.h"

AQuinnCharacter::AQuinnCharacter()
{
	m_fireCooldown = 0.0f;
	m_slamCooldown = 0.0f;
	FireTotalCooldown = 0.4f;
	SlamTotalCooldown = 1.0f;
	ProjectileSpeed = 750.0f;
	HeadJumpDamage = 25.0f;
	SlamMultiplier = 1.35f;
	ProjectileDamageMinMax = FVector2D(10, 25);
	m_currentDmgMultiplier = 1.0f;
	m_isInvulnerable = false;

	CurrentLives = 0;
	StartLives = 3;
	LastCheckpoint = nullptr;		// Nullptr to start with

	// Set hit/kill score to 0 as it's the player character
	KilledScore = HitScore = 0;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate when the controller rotates.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom attached to the root (capsule)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bAbsoluteRotation = true; // Rotation of the character should not affect rotation of boom
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->TargetArmLength = 900.f;
	CameraBoom->SocketOffset = FVector(0.f, 0.f, 75.f);
	CameraBoom->RelativeRotation = FRotator(0.f, 180.f, 0.f);

	// Create a camera and attach to boom
	SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
	SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	SideViewCameraComponent->bUsePawnControlRotation = false; // We don't want the controller rotating the camera
	SideViewCameraComponent->SetFieldOfView(80.0f);

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1250.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	GetCharacterMovement()->MaxFlySpeed = 600.f;

	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AQuinnCharacter::OnStompCapsuleBeginOverlap);
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);

	// Configure Stomp collider to be placed below the character
	StompSphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SlamCapsule"));
	StompSphereComponent->SetupAttachment(RootComponent);
	StompSphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AQuinnCharacter::OnStompCapsuleBeginOverlap);
	// Overlap with all channels cause setting a profile name doesnt work
	StompSphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	StompSphereComponent->SetGenerateOverlapEvents(true);
	StompSphereComponent->AddLocalOffset(FVector(0, 0, -100.0f));
	StompSphereComponent->SetSphereRadius(44.0f);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void AQuinnCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
}

void AQuinnCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentLives = StartLives;
	SetDestroyOnFall(false);
}

void AQuinnCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Lock the X axis to 0
	FVector actorLocation = GetActorLocation();
	SetActorLocation(FVector(0, actorLocation.Y, actorLocation.Z));

	// Reduce cooldown every tick
	if (m_fireCooldown > 0)
		m_fireCooldown -= DeltaTime;
	if (m_slamCooldown > 0)
		m_slamCooldown -= DeltaTime;

	if (m_isSlamingGround && !GetCharacterMovement()->IsFalling())
	{
		UE_LOG(LogTemp, Log, TEXT("Slam finished"));
		m_isSlamingGround = false;
	}
}

void AQuinnCharacter::Jump()
{
	// Set flags to false on jump init
	m_hasAppliedDmgThisJump = m_isSlamingGround = false;

	Super::Jump();
}

void AQuinnCharacter::StopJumping()
{
	Super::StopJumping();
}

float AQuinnCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (this->GetIsInvulnerable())
	{
		return 0;
	}
	
	return damage;
}

void AQuinnCharacter::MoveRight(float Value)
{
	// add movement in that direction
	AddMovementInput(FVector(0.f, -1.f, 0.f), Value);
}

bool AQuinnCharacter::OnCharacterDeath()
{
	bool isDead = false;

	// Check if all lives have been used
	if (CurrentLives > 0)
	{
		CurrentLives -= 1;

		isDead = CurrentLives <= 0;
	}

	if (isDead)
	{
		UE_LOG(LogTemp, Log, TEXT("Quinn has no lives remaining"));
		
		// Call super on actual death
		Super::OnCharacterDeath();

		// Clear lifespan timer to not destroy character as it gets set in super
		SetLifeSpan(0.0f);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Quinn lost a life (%d/%d remaining). Respawning"), CurrentLives, StartLives);

		// Set health back to full
		SetCurrentHealth(GetTotalHealth());

		// Clear any velocity
		GetCharacterMovement()->Velocity = FVector::ZeroVector;

		// Set actors location to start location
		ACheckpoint* checkpoint = GetLastCheckpoint();
		if (checkpoint)
		{
			// Set position to last checkpoint
			SetActorLocation(checkpoint->GetActorLocation());
		}
		else
		{
			// Set to spawn location
			// Needs changing to variable
			SetActorLocation(FVector(0, 110, 230));
		}

		// Clear any effects character has from power-ups
		ClearCharacterEffects();
	}

	return isDead;
}

void AQuinnCharacter::FireProjectile(FVector direction)
{
	// Check if firing is currently on cooldown or not
	if (m_fireCooldown > 0)
	{
		return;
	}

	// Check projectile has been set
	if (!ProjectileActor)
	{
		UE_LOG(LogTemp, Error, TEXT("No Projectile actor has been set!"));
		return;
	}

	/// Debug
	//UE_LOG(LogTemp, Log, TEXT("Firing projectile!"));
	//DrawDebugSphere(GetWorld(), spawnLocation, 20.0f, 4, FColor::Green, false, 5.0f, 0, 2.0f);

	// Spawn blueprint template projectile actor
	AActor* projectileActor = GetWorld()->SpawnActor(ProjectileActor->GetDefaultObject()->GetClass());
	
	// Cast to projectile and configure
	AProjectileBase* projectile = Cast<AProjectileBase>(projectileActor);
	if (IsValid(projectile))
	{
		// Projectile should ignore class that fires it
		projectile->AddActorToIgnore(this);
		// Set speed and life span
		projectile->SetSpeed(ProjectileSpeed);
		projectile->SetLifeSpan(5.0f);
		// Launch in direction
		projectile->FireInDirection(direction);

		// Randomly determine damage amount for projectile, remove decimals
		float rndDmg = FMath::RandRange(ProjectileDamageMinMax.X, ProjectileDamageMinMax.Y);
		rndDmg = FMath::FloorToInt(rndDmg);
		
		// Multiply damage by multiplier
		rndDmg *= this->GetDamageMultiplier();

		// Set damage on projectile
		projectile->SetDamage(rndDmg);

		FVector halfLocation = GetHalfLocationWithOffset(0.0f);
		projectileActor->SetActorLocation(halfLocation, false);
	}

	// Set firing on cooldown
	m_fireCooldown = FireTotalCooldown;
}


void AQuinnCharacter::SlamGround()
{
	// Check if slam is on cooldown
	if (m_slamCooldown > 0)
	{
		return;
	}

	// Slam down direction with force
	FVector downDirection = -GetActorUpVector();
	float slamForce = 2000.0f;
	LaunchCharacter(downDirection * slamForce, false, true);

	// Set on cooldown and slam flag
	m_slamCooldown = SlamTotalCooldown;
	m_isSlamingGround = true;
}

int AQuinnCharacter::GetCurrentLives()
{
	return CurrentLives;
}

void AQuinnCharacter::AddLives(int amount)
{
	// Add lives amount
	CurrentLives += amount;

	// If current is more than total, set to total
	if (CurrentLives > GetTotalLives())
		CurrentLives = GetTotalLives();
}

int AQuinnCharacter::GetTotalLives()
{
	return StartLives;
}

ACheckpoint* AQuinnCharacter::GetLastCheckpoint()
{
	return LastCheckpoint;
}

void AQuinnCharacter::SetCheckpoint(ACheckpoint* checkpoint)
{
	LastCheckpoint = checkpoint;
}

void AQuinnCharacter::SetIsInvulnerable(bool isInvulnerable)
{
	m_isInvulnerable = isInvulnerable;
}

bool AQuinnCharacter::GetIsInvulnerable()
{
	return m_isInvulnerable;
}

void AQuinnCharacter::OnStompCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	/*
	* Player stomps on head of another character
	*/

	// Dont apply damage to self
	if (OtherActor == this) 
	{
		return;
	}

	// Check if actor is an enemy that can take damage
	if (OtherActor->IsA(AHealthCharacter::StaticClass()))
	{
		AHealthCharacter* healthChar = Cast<AHealthCharacter>(OtherActor);
		// If no damage has been applied this jump...
		if (!m_hasAppliedDmgThisJump)
		{
			// Calculate damage for a stomp
			float dmg = HeadJumpDamage;
			if (m_isSlamingGround)
			{
				// Add slam multiplier to damage
				dmg *= SlamMultiplier;

				// Collided with enemy, end slam
				m_isSlamingGround = false;
			}

			// Multiply damage by multiplier
			dmg *= this->GetDamageMultiplier();

			// Deal stomp damage to the character
			bool dealtDmg = healthChar->TakeStomp(dmg);
			if (dealtDmg)
			{
				UE_LOG(LogTemp, Log, TEXT("Dealt '%f' damage to '%s' since stomped on head"), dmg, *OtherActor->GetName());
				m_hasAppliedDmgThisJump = true;
			}
		}
	}
}

void AQuinnCharacter::SetIsInvulnerableForDuration(int duration)
{
	// Check timer isnt active
	if (!GetWorldTimerManager().IsTimerActive(m_invulnerabilityTimerHandle))
	{
		// Set character to invulnerable
		this->SetIsInvulnerable(true);

		// Start timer for duration
		GetWorldTimerManager().SetTimer(m_invulnerabilityTimerHandle, this, &AQuinnCharacter::OnInvulnerabilityDurationExpired, duration);
		UE_LOG(LogTemp, Log, TEXT("Quinn has started invulnerability for '%d' seconds!"), duration);
	}
}

void AQuinnCharacter::SetDamageMultiplierForDuration(float multiplier, int duration)
{
	if (!GetWorldTimerManager().IsTimerActive(m_dmgMultiplierTimerHandle))
	{
		this->SetDamageMultiplier(multiplier);

		GetWorldTimerManager().SetTimer(m_dmgMultiplierTimerHandle, this, &AQuinnCharacter::OnDmgMultiplierDurationExpired, duration);
		UE_LOG(LogTemp, Log, TEXT("Set Dmg Multiplier to '%f' for '%d' seconds"), multiplier, duration);
	}
}

float AQuinnCharacter::GetDamageMultiplier()
{
	return m_currentDmgMultiplier;
}

void AQuinnCharacter::SetDamageMultiplier(float multiplier)
{
	// Dont let multiplier be less than 0.5x
	if (multiplier <= 0.5f)
	{
		m_currentDmgMultiplier = 0.5f;
	}
	else
	{
		// Set the given multiplier
		m_currentDmgMultiplier = multiplier;
	}
}

void AQuinnCharacter::OnInvulnerabilityDurationExpired()
{
	// Disable invulnerability after duration
	UE_LOG(LogTemp, Log, TEXT("Quinn invulnerability expired"));
	this->SetIsInvulnerable(false);
}

void AQuinnCharacter::OnDmgMultiplierDurationExpired()
{
	UE_LOG(LogTemp, Log, TEXT("Finished dmg multiplier. Set mutliplier to 1.0f"));
	this->SetDamageMultiplier(1.0f);
}

void AQuinnCharacter::ClearCharacterEffects()
{
	// Check if invulnerability timer is active and clear it
	if (GetWorldTimerManager().IsTimerActive(m_invulnerabilityTimerHandle))
	{
		GetWorldTimerManager().ClearTimer(m_invulnerabilityTimerHandle);
	}

	// Disable invulnerability if active
	if (this->GetIsInvulnerable())
	{
		this->SetIsInvulnerable(false);
	}
}
