// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuHUD.h"
#include "Blueprint/UserWidget.h"

AMainMenuHUD::AMainMenuHUD()
{

}

void AMainMenuHUD::BeginPlay()
{
	Super::BeginPlay();

	// If menu widgets contains items that need to be created
	if (MenuWidgets.Num() > 0)
	{
		// Iterate through each widget
		for (TSubclassOf<UUserWidget> widgetClass : MenuWidgets)
		{
			// Create, add to viewport and to tracking array
			UUserWidget* createdWidget = CreateWidget<UUserWidget>(GetWorld(), widgetClass);
			if (IsValid(createdWidget))
			{
				createdWidget->AddToViewport();
				m_createdMenuWidgets.Add(createdWidget);
			}
		}
	}
}
