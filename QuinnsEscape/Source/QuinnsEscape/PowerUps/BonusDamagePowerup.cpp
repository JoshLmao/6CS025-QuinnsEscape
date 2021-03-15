// Fill out your copyright notice in the Description page of Project Settings.


#include "BonusDamagePowerup.h"
#include "../Characters/QuinnCharacter.h"

ABonusDamagePowerup::ABonusDamagePowerup()
{
	DamageMultiplier = 1.5;
	Duration = 20;
}

void ABonusDamagePowerup::ApplyEffects(APawn* collidedPawn)
{
	Super::ApplyEffects(collidedPawn);

	if (IsValid(collidedPawn) && collidedPawn->IsA(AQuinnCharacter::StaticClass()))
	{
		AQuinnCharacter* quinn = Cast<AQuinnCharacter>(collidedPawn);
		quinn->SetDamageMultiplierForDuration(DamageMultiplier, Duration);
	}
}
