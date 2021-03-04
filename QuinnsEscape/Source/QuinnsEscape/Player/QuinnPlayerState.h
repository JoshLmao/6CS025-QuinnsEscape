// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "QuinnPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class QUINNSESCAPE_API AQuinnPlayerState : public APlayerState
{
	GENERATED_BODY()
	
	AQuinnPlayerState();

public:
	float Score;
};
