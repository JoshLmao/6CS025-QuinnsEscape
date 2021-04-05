


#include "QuinnGameplayStatics.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"

QuinnGameplayStatics::QuinnGameplayStatics()
{
}

QuinnGameplayStatics::~QuinnGameplayStatics()
{
}

void QuinnGameplayStatics::PlaySound(UAudioComponent* audioComp, USoundBase* sound, float pitch, float volume)
{
	// Check audio component and sound are valid
	if (IsValid(audioComp) && IsValid(sound))
	{
		// Set modifiers
		audioComp->SetPitchMultiplier(pitch);
		audioComp->SetVolumeMultiplier(volume);
		// Set sound and play
		audioComp->SetSound(sound);
		audioComp->Play();
	}
}

void QuinnGameplayStatics::PlaySoundRndPitch(UAudioComponent* audioComp, USoundBase* sound, float min, float max)
{
	// Get a random pitch and play sound
	float rndPitch = FMath::RandRange(min, max);
	QuinnGameplayStatics::PlaySound(audioComp, sound, rndPitch);
}
