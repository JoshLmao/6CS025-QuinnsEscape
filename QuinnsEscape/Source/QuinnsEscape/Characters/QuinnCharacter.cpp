// Fill out your copyright notice in the Description page of Project Settings.


#include "QuinnCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "../Projectiles/ProjectileBase.h"

AQuinnCharacter::AQuinnCharacter()
{
	m_fireCooldown = 0.0f;
	m_slamCooldown = 0.0f;
	FireTotalCooldown = 0.4f;
	SlamTotalCooldown = 1.0f;
	ProjectileSpeed = 750.0f;
	HeadJumpDamage = 25.0f;
	SlamMultiplier = 1.35f;

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
	GetCharacterMovement()->JumpZVelocity = 1000.f;
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
	// Actions
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AQuinnCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	//PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AQuinnCharacter::FireProjectile);

	// Axis
	PlayerInputComponent->BindAxis("MoveRight", this, &AQuinnCharacter::MoveRight);
}

void AQuinnCharacter::BeginPlay()
{
	Super::BeginPlay();

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

void AQuinnCharacter::MoveRight(float Value)
{
	// add movement in that direction
	AddMovementInput(FVector(0.f, -1.f, 0.f), Value);
}

void AQuinnCharacter::FireProjectile(FVector direction)
{
	// Check if firing is currently on cooldown or not
	if (m_fireCooldown > 0)
	{
		return;
	}

	//UE_LOG(LogTemp, Log, TEXT("Firing projectile!"));

	// Create spawn location with offset and spawn into world
	float offset = 100.0f;
	FVector forwardVector = direction - GetActorLocation();
	forwardVector.Normalize();

	FVector spawnLocation = GetActorLocation() + (forwardVector * offset);
	spawnLocation.X = GetActorLocation().X;
	spawnLocation.Z += 50.0f; // add height offset

	AActor* projectileActor = GetWorld()->SpawnActor<AActor>(AProjectileBase::StaticClass(), FVector(), FRotator(), FActorSpawnParameters());
	
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

		projectileActor->SetActorLocation(spawnLocation);
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
