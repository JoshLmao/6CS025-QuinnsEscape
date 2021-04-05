

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class QUINNSESCAPE_API QuinnGameplayStatics
{
public:
	QuinnGameplayStatics();
	~QuinnGameplayStatics();

public:
	// Plays the given sound on the character's audio component.
	static void PlaySound(class UAudioComponent* audioComp, class USoundBase* sound, float pitch = 1.0f, float volume = 1.0f);
	// Plays the given sound with a random pitch inbetween the given values
	static void PlaySoundRndPitch(class UAudioComponent* audioComp, class USoundBase* sound, float min, float max);
};
