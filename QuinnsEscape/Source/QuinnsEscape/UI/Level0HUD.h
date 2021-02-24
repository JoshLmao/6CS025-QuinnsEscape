// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Level0HUD.generated.h"

/**
 * 
 */
UCLASS()
class QUINNSESCAPE_API ALevel0HUD : public AHUD
{
	GENERATED_BODY()
	
public:
	ALevel0HUD();

	/*
	*	 VARIABLES
	*/
protected:
	// All on screen widgets to add to the HUD on BeginPlay
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class UUserWidget>> AllUIWidgets;

private:
	// Current list of created UI widgets that are constantly active on screen
	TArray<class UUserWidget*> m_createdWidgets;

	/*
	*	 METHODS
	*/
protected:
	virtual void BeginPlay() override;
	virtual void DrawHUD() override;

};
