// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "QuinnUIWidget.generated.h"

/**
 *	Widget for interfacing with the AQuinnCharacter that exists in the scene.
 */
UCLASS()
class QUINNSESCAPE_API UQuinnUIWidget : public UUserWidget
{
	GENERATED_BODY()
	
	/*
	*	VARIABLES
	*/
private:
	// Reference to the Quinn character
	class AQuinnCharacter* m_quinn;

	/*
	*	METHODS
	*/
protected:
	// Gets the Quinn character
	UFUNCTION(BlueprintCallable, Category = "Quinn")
	virtual class AQuinnCharacter* GetQuinnCharacter();

	// Gets the current health
	UFUNCTION(BlueprintCallable, Category = "Quinn")
	virtual float GetCurrentHealth();
	// Gets the total health
	UFUNCTION(BlueprintCallable, Category = "Quinn")
	virtual float GetTotalHealth();

	// Gets the current amount of lives Quinn has
	UFUNCTION(BlueprintCallable, Category = "Quinn")
	virtual int GetCurrentLives();
	// Gets the total amount of lives Quinn has
	UFUNCTION(BlueprintCallable, Category = "Quinn")
	virtual int GetTotalLives();

	// Called when Widget gets constructed
	virtual void NativeConstruct() override;

	// Adds blank or filled hearts to the given canvas panel with right/center alignment. 
	// Requires the (Blueprint) class of the blank and filled heart widgets created in UMG.
	UFUNCTION(BlueprintCallable, Category = "Quinn")
	virtual void AddLivesToWidget(class UCanvasPanel* panel, class UClass* blankHeartWidget, class UClass* filledHeartWidget);
};
