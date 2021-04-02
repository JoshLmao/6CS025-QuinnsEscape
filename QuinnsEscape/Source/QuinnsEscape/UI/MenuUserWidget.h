

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class QUINNSESCAPE_API UMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
	/*
	*	METHODS
	*/
public:
	// Gets the current version of the project, set in project settings
	UFUNCTION(BlueprintCallable)
	FString GetProjectVersion();
};
