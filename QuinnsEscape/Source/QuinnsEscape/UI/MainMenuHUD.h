// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainMenuHUD.generated.h"

/**
 *	HUD used for the main menu. 
 *	Simply spawns the given array of UUserWidgets
 */
UCLASS()
class QUINNSESCAPE_API AMainMenuHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	AMainMenuHUD();

	/*
	*	VARIABLES
	*/
public:
	// Array of widgets that should be created and displayed to viewport on begin
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class UUserWidget>> MenuWidgets;

private:
	// Array of created widgets by the HUD
	TArray<class UUserWidget*> m_createdMenuWidgets;

	/*
	*	METHODS
	*/
public:
	// Called when HUD begins to play
	virtual void BeginPlay() override;
};
