// Fill out your copyright notice in the Description page of Project Settings.


#include "ExtraLifePowerup.h"
#include "GameFramework/Pawn.h"
#include "../Characters/QuinnCharacter.h"
#include "../Game/QuinnGameState.h"

AExtraLifePowerup::AExtraLifePowerup()
{
	ExtraLivesAmount = 1;
	BonusScore = 100;
}

void AExtraLifePowerup::ApplyEffects(APawn* collidedPawn)
{
	Super::ApplyEffects(collidedPawn);
	
	// Check pawn is valid and is QuinnCharacter
	if (IsValid(collidedPawn) && collidedPawn->IsA(AQuinnCharacter::StaticClass()))
	{
		// Cast to Quinn and add extra lives
		AQuinnCharacter* quinnChar = Cast<AQuinnCharacter>(collidedPawn);
		if (quinnChar->GetCurrentLives() < quinnChar->GetTotalLives())
		{
			quinnChar->AddLives(ExtraLivesAmount);
			UE_LOG(LogTemp, Log, TEXT("Gave Quinn '%d' extra hearts. Current: '%d' Total: '%d'"), ExtraLivesAmount, quinnChar->GetCurrentLives(), quinnChar->GetTotalLives());
		}
		else
		{
			// Reward with score since hearts are full
			UE_LOG(LogTemp, Log, TEXT("Quinn at full hearts, rewarding with '%d' score!"), BonusScore);
			if (AQuinnGameState* quinnGameState = this->GetQuinnGameState())
			{
				quinnGameState->AddScore(BonusScore);
			}
		}
	}
}