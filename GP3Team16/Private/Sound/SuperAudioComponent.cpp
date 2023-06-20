#include "Sound/SuperAudioComponent.h"
#include "Sound/SoundPriorityDataAsset.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

// INITIALISATION

	USuperAudioComponent::USuperAudioComponent(){
		bAutoActivate = false;
	}

	void USuperAudioComponent::BeginPlay(){
		Super::BeginPlay();
		if (!SoundPriority) return;
		
		bAllowSpatialization = SoundPriority->AllowsSpatialisation();
		VolumeMultiplier	 = SoundPriority->GetVolumeMultiplier();

		AttenuationSettings  = SoundPriority->GetAttenuationSettings();
		bOverrideAttenuation = AttenuationSettings != nullptr;
	}

// AUDIO HANDLING

	void USuperAudioComponent::Play(float StartTime){
		if (!Sound || !HasExceededTime() || !HasReachedCount()) return;

		SetPitchShift();
		
		Super::Play(StartTime);
	}

	const bool USuperAudioComponent::HasExceededTime(){
		if (!UseTimeThreshold) return true;

		const float Time = UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld());
		if (Time < PreviousPlayDateTime + MinTimeBetweenPlay) return false;

		PreviousPlayDateTime = Time;
		return true;
	}

	const bool USuperAudioComponent::HasReachedCount(){
		if (!UseCountThreshold) return true;

		if (++PlayCount < PlayCountThreshold)
			return false;

		PlayCountThreshold = UKismetMathLibrary::RandomIntegerInRange(
			MinPlayCountThreshold, MaxPlayCountThreshold
		);

		PlayCount = 0;
		return true;
	}

	void USuperAudioComponent::SetPitchShift(){
		if (!UsePitchShift) return;

		PitchMultiplier = UKismetMathLibrary::RandomFloatInRange(
			MinPitchShift, MaxPitchShift
		);
	}
