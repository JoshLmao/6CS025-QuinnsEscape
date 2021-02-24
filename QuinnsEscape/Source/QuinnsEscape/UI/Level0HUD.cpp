// Fill out your copyright notice in the Description page of Project Settings.


#include "Level0HUD.h"
#include "Blueprint/UserWidget.h"

ALevel0HUD::ALevel0HUD()
{
}

void ALevel0HUD::BeginPlay()
{
	Super::BeginPlay();

	// If any widgets need to be added
	if (AllUIWidgets.Num() > 0)
	{
		// Iterate through all widgets
		for (TSubclassOf<UUserWidget> widget : AllUIWidgets)
		{
			// Create an instance of the widget and add to viewport
			UUserWidget* createdWidget = CreateWidget<UUserWidget>(GetWorld(), widget);
			createdWidget->AddToViewport();

			// Store instanced widget in array
			m_createdWidgets.Add(createdWidget);
		}
	}
}

void ALevel0HUD::DrawHUD()
{
	Super::DrawHUD();

}