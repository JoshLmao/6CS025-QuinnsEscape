// Fill out your copyright notice in the Description page of Project Settings.


#include "CoinBonusPowerup.h"
#include "../Characters/QuinnCharacter.h"

void ACoinBonusPowerup::ApplyEffects(APawn* pawn)
{
	Super::ApplyEffects(pawn);

	// Cast to quinn
	if (AQuinnCharacter* quinn = Cast<AQuinnCharacter>(pawn))
	{
		// To Do: set effects for coin bonus
	}
}