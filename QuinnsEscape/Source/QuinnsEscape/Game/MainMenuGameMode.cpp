


#include "MainMenuGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "UI/MainMenuHUD.h"

AMainMenuGameMode::AMainMenuGameMode()
{
	// Set HUD class a AMainMenuHUD.
	// Should be overridden by a blueprinted version though
	HUDClass = AMainMenuHUD::StaticClass();
}

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Get Player Controller
	APlayerController* pc = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (pc)
	{
		pc->bShowMouseCursor = true;	// Display mouse cursor to click UI elements
		pc->SetIgnoreLookInput(true);	// Disable look input
		pc->SetIgnoreMoveInput(true);	// Disable movement input
	}
}