// Fill out your copyright notice in the Description page of Project Settings.


#include "QuinnCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "../Projectiles/ProjectileBase.h"

AQuinnCharacter::AQuinnCharacter()
{
	m_fireCooldown = 0.0f;
	FireTotalCooldown = 0.4f;

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

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void AQuinnCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	// Actions
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
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

	// Reduce cooldown every tick
	if (m_fireCooldown > 0)
		m_fireCooldown -= DeltaTime;
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
	float offset = 75.0f;
	FVector spawnLocation = GetActorLocation() + (GetActorForwardVector() * offset);
	AActor* projectileActor = GetWorld()->SpawnActor<AActor>(AProjectileBase::StaticClass(), spawnLocation, FRotator(), FActorSpawnParameters());
	
	// Cast to projectile and configure
	AProjectileBase* projectile = Cast<AProjectileBase>(projectileActor);
	if (IsValid(projectile))
	{
		// Projectile should ignore class that fires it
		projectile->AddActorToIgnore(this);
		// Set speed and life span
		projectile->SetSpeed(250.0f);
		projectile->SetLifeSpan(5.0f);
		// Launch in direction
		projectile->FireInDirection(direction);
	}

	// Set firing on cooldown
	m_fireCooldown = FireTotalCooldown;
}
