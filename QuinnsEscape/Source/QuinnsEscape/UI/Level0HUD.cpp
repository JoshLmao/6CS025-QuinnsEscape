// Fill out your copyright notice in the Description page of Project Settings.


#include "Level0HUD.h"
#include "Blueprint/UserWidget.h"

ALevel0HUD::ALevel0HUD()
{
}

void ALevel0HUD::BeginPlay()
{
	Super::BeginPlay();

	// Create gameplay and end screen widgets
	m_createdGameplayWidgets = CreateAllWidgets(GameplayUIWidgets, true);
	m_createdLvlCompleteWidgets = CreateAllWidgets(LevelCompleteWidgets);
	m_createdLvlFailedWidgets = CreateAllWidgets(LevelFailWidgets);
}

void ALevel0HUD::SetHUDDisplayState(EHUDState hudState)
{
	// Set the correct widget on viewport states depending on state
	SetWidgetsToViewport(m_createdGameplayWidgets, hudState == EHUDState::Gameplay);
	SetWidgetsToViewport(m_createdLvlCompleteWidgets, hudState == EHUDState::LevelComplete);
	SetWidgetsToViewport(m_createdLvlFailedWidgets, hudState == EHUDState::LevelFailed);
}

TArray<class UUserWidget*> ALevel0HUD::CreateAllWidgets(TArray<TSubclassOf<UUserWidget>> widgets, bool addToViewport)
{
	TArray<UUserWidget*> allWidgets;

	if (widgets.Num() > 0)
	{
		// Iterate through all widgets
		for (TSubclassOf<UUserWidget> widget : widgets)
		{
			// Create an instance of the widget and add to viewport
			UUserWidget* createdWidget = CreateWidget<UUserWidget>(GetWorld(), widget);
			
			// Add widget to viewport if needed
			if (IsValid(createdWidget) && addToViewport)
			{
				createdWidget->AddToViewport();
			}

			// Store instanced widget in array
			allWidgets.Add(createdWidget);
		}
	}

	return allWidgets;
}

void ALevel0HUD::SetWidgetsToViewport(TArray<class UUserWidget*> instWidgets, bool addToViewport)
{
	for (UUserWidget* widget : instWidgets)
	{
		if (addToViewport)
		{
			widget->AddToViewport();
		}
		else
		{
			widget->RemoveFromViewport();
		}
	}
}