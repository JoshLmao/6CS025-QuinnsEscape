// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Game/QEPlayerSaveData.h"
#include "QuinnUIWidget.generated.h"

//DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuinnWidgetIsInViewportChangedSignature, bool, IsInViewport);

/**
 *	Widget for display UI elements relevant to Quinns Escape. 
 *	Contains code to retrieve save data, Quinn character data and Score data
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
	// Reference to game state
	class AQuinnGameState* QuinnGameState;
	// Last state of if the widget is visible on viewport
	bool m_lastIsVisibleToViewport;

	/*
	*	EVENTS
	*/
public:
	// Event for when IsInViewport has changed
	// Blueprint Event
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void IsInViewportChanged(bool isInViewport);

	/*
	*	METHODS
	*/
protected:
	// Called when Widget gets constructed
	virtual void NativeConstruct() override;
	// Called every tick
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// Gets the Quinn character
	UFUNCTION(BlueprintCallable, Category = "Quinn")
	virtual class AQuinnCharacter* GetQuinnCharacter();
	// Gets the current game state in the world, cast to AQuinnGameState
	UFUNCTION(BlueprintCallable)
	class AQuinnGameState* GetQuinnGameState();

	// Gets the current health
	UFUNCTION(BlueprintCallable, Category = "Quinn")
	float GetCurrentHealth();
	// Gets the current health , rounded to the nearest whole number
	UFUNCTION(BlueprintCallable, Category = "Quinn")
	int GetCurrentHealthRounded();
	// Gets the total health
	UFUNCTION(BlueprintCallable, Category = "Quinn")
	float GetTotalHealth();

	// Gets the current amount of lives Quinn has
	UFUNCTION(BlueprintCallable, Category = "Quinn")
	virtual int GetCurrentLives();
	// Gets the total amount of lives Quinn has
	UFUNCTION(BlueprintCallable, Category = "Quinn")
	virtual int GetTotalLives();

	// Gets the current score
	UFUNCTION(BlueprintCallable)
	float GetCurrentScore();
	// Gets the current multiplier
	UFUNCTION(BlueprintCallable)
	float GetCurrentScoreMultiplier();

	// Gets the time in seconds the Quinn character has been alive
	UFUNCTION(BlueprintCallable)
	float GetAliveDurationSeconds();
	// Converts a seconds duration to a time (minute/seconds) format such as 00:00
	UFUNCTION(BlueprintCallable)
	static FString ConvertSecondsToTimeFormat(float seconds);

	// Adds blank or filled hearts to the given canvas panel with right/center alignment. 
	// Requires the (Blueprint) class of the blank and filled heart widgets created in UMG.
	UFUNCTION(BlueprintCallable, Category = "Quinn")
	virtual void AddLivesToWidget(class UCanvasPanel* panel, class UClass* blankHeartWidget, class UClass* filledHeartWidget);

	// gets all high scores in save game
	UFUNCTION(BlueprintCallable)
	TArray<FQESingleGameData> GetAllHighScores();
};
