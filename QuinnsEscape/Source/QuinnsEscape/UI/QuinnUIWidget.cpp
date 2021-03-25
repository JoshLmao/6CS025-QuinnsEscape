// Fill out your copyright notice in the Description page of Project Settings.


#include "QuinnUIWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CanvasPanel.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanelSlot.h"

#include "../Characters/QuinnCharacter.h"
#include "../Game/QuinnGameState.h"
#include "../Game/QEPlayerSaveData.h"

void UQuinnUIWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// Find quinn character on widget construct
	TArray<AActor*> foundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AQuinnCharacter::StaticClass(), foundActors);
	if (foundActors.Num() > 0)
	{
		m_quinn = Cast<AQuinnCharacter>(foundActors[0]);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Widget '%s' unable to get reference to Quinn character!"), *GetName());
	}
	
	// Get game state
	AGameStateBase* state = GetWorld()->GetGameState();
	if (state != nullptr)
	{
		QuinnGameState = Cast<AQuinnGameState>(state);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to determine QuinnGameState. UI Widget won't work!"));
	}
}

void UQuinnUIWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Detect if changed since last tick
	if (m_lastIsVisibleToViewport != this->IsInViewport())
	{
		// Trigger event for viewport changed
		IsInViewportChanged(this->IsInViewport());
	}

	// Update to new state
	m_lastIsVisibleToViewport = this->IsInViewport();
}

AQuinnCharacter* UQuinnUIWidget::GetQuinnCharacter()
{
	return m_quinn;
}

float UQuinnUIWidget::GetCurrentHealth()
{
	if (IsValid(m_quinn)) {
		return m_quinn->GetCurrentHealth();
	}
	return 0.0f;
}

float UQuinnUIWidget::GetTotalHealth()
{
	if (IsValid(m_quinn)) {
		return m_quinn->GetTotalHealth();
	}
	return 0;
}

int UQuinnUIWidget::GetCurrentLives()
{
	if (IsValid(m_quinn)) {
		return m_quinn->GetCurrentLives();
	}
	return 0;
}

int UQuinnUIWidget::GetTotalLives()
{
	if (IsValid(m_quinn)) {
		return m_quinn->GetTotalLives();
	}
	return 0;
}

void UQuinnUIWidget::AddLivesToWidget(UCanvasPanel* panel, UClass* blankHeartWidget, UClass* filledHeartWidget)
{
	if (!panel || !blankHeartWidget || !filledHeartWidget)
		return;

	// Clear any children first
	panel->ClearChildren();

	// get total and current lives
	int total = GetTotalLives();
	int current = GetCurrentLives();

	for (int i = 0; i < total; i++)
	{
		// Instantiate the correct widget, depending on if current iteration is empty or filled heart
		UUserWidget* heart;
		if ((i + 1) <= current)
		{
			heart = CreateWidget<UUserWidget>(GetWorld(), filledHeartWidget);
		}
		else
		{
			heart = CreateWidget<UUserWidget>(GetWorld(), blankHeartWidget);
		}

		// Add new widget to panel as a child
		UPanelSlot* panelSlot = panel->AddChild(heart);

		// Cast panel slot to canvas panel slot
		UCanvasPanelSlot* slot = Cast<UCanvasPanelSlot>(panelSlot);
		if (!slot)
		{
			// unable to cast
			UE_LOG(LogTemp, Error, TEXT("Unable to cast current PanelSlot to CanvasPanelSlot"));
			continue;
		}

		// Set anchor and reset offset/position to blank
		slot->SetAnchors(FAnchors(1.0f, 0.5f, 1.0f, 0.5f));		// Middle Right
		slot->SetOffsets(FMargin());
		slot->SetPosition(FVector2D());
		slot->SetAlignment(FVector2D(1.0f, 0.5f));				// Set to stretch to height, top right corner (Also known as pivot point)

		slot->SetSize(FVector2D(50.0f, 50.0f));					// Image size to 50px * 50px

		// Determine X pos of current widget
		float xPadding = 10.0f;
		FVector2D imgSize = slot->GetSize();
		float xPosition = (imgSize.X * i) + xPadding;

		// Set it's position. No Y as Alignment does the work for us
		slot->SetPosition(FVector2D(-xPosition, 0.0f));
	}
}

AQuinnGameState* UQuinnUIWidget::GetQuinnGameState()
{
	return QuinnGameState;
}

float UQuinnUIWidget::GetCurrentScore()
{
	if (IsValid(QuinnGameState))
	{
		return QuinnGameState->GetScore();
	}
	return 0;
}

float UQuinnUIWidget::GetCurrentScoreMultiplier()
{
	if (IsValid(QuinnGameState))
	{
		return QuinnGameState->GetScoreMultiplier();
	}
	return 1.0f;
}

float UQuinnUIWidget::GetAliveDurationSeconds()
{
	if (m_quinn) {
		return m_quinn->GetCharacterAliveDuration();
	}
	return 0;
}

FString UQuinnUIWidget::ConvertSecondsToTimeFormat(float seconds)
{
	// Determine seconds by modulus (%) seconds by 60
	int secs = FMath::Fmod(seconds,  60);
	// Determine mins by dividing current seconds by 60
	int mins = seconds / 60;
	
	// Add 0 before minutes if less than 10
	FString minsStr = "";
	if (mins < 10)
	{
		minsStr += "0";
	}
	// Convert float to int to remove decimal
	minsStr += FString::FromInt((int)mins);

	// Add 0 before seconds if less than 10
	FString secsStr = "";
	if (secs < 10)
	{
		secsStr += "0";
	}
	// Convert float to int to remove decimal
	secsStr += FString::FromInt((int)secs);

	// Return format of "00:00"
	return minsStr + ":" + secsStr;
}

TArray<FQESingleGameData> UQuinnUIWidget::GetAllHighScores()
{
	auto ptr = UQEPlayerSaveData::GetCurrentSaveData();
	if (IsValid(ptr))
	{
		return ptr->GetHighScores();
	}
	return TArray<FQESingleGameData>();
}
