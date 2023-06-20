#include "Sound/PlaySoundComponent.h"
#include "GP3Team16/GP3Team16GameModeBase.h"
#include "Sound/SoundManager.h"
#include "Sound/SoundDataAsset.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

// INITIALISATION

	UPlaySoundComponent::UPlaySoundComponent(){
		PrimaryComponentTick.bStartWithTickEnabled = false;
		PrimaryComponentTick.bCanEverTick = false;
		bAutoActivate = true;
	}

	void UPlaySoundComponent::BeginPlay(){
		Super::BeginPlay();

		Owner = GetOwner();
		SoundLocation = Owner->GetActorLocation();
		
		InitAudioComponent();
		InitSoundManager();
	}

	void UPlaySoundComponent::InitAudioComponent(){
		AudioComponent = Cast<UAudioComponent>(
			Owner->GetComponentByClass(UAudioComponent::StaticClass())
		);
		if (!AudioComponent) UE_LOG(LogTemp, Fatal, TEXT("Missing Reference: AudioComponent"))
	}

	void UPlaySoundComponent::InitSoundManager(){
		const auto* GameMode = Cast<AGP3Team16GameModeBase>(
			UGameplayStatics::GetGameMode(GetWorld())
		);
		if (!GameMode) UE_LOG(LogTemp, Fatal, TEXT("Missing Reference: AGP3Team16GameModeBase"));
		SoundManager = GameMode->GetSoundManager();

		if (!SoundManager) UE_LOG(LogTemp, Fatal, TEXT("Missing Reference: SoundManager"));
	}

// SOUND HANDLING

	void UPlaySoundComponent::PlaySound(const uint8 Index){
		//if (Index < Sounds.Num())
			//PlaySound(Sounds[Index]);
	}

	void UPlaySoundComponent::PlaySoundAsset(USoundDataAsset* Sound, FIntVector2& PlayCountAndThreshold){
		if (!Sound->GetSound()) return;

		// Ignore this if high priority
		// Also check if previous sound has higher priority than this one
		// Also don't increment count if high priority
		//if (++CurrentPlayCount < PlayCountThreshold)
		
		if (Sound->IsDynamic()) SoundLocation = Owner->GetActorLocation();
		if (!SoundManager->RequestToPlaySound(Sound->GetPriority(), SoundLocation)) return;
		
		AudioComponent->VolumeMultiplier = SoundManager->GetVolume(
			Sound->GetPriority(), SoundLocation
		);
		AudioComponent->PitchMultiplier = FMath::RandRange(
			Sound->GetMinPitchShift(), Sound->GetMaxPitchShift()
		);
		AudioComponent->Sound = Sound->GetSound();
		AudioComponent->Play();
		
		PlayCountAndThreshold.Y = FMath::RandRange(
			Sound->GetMinPlayThreshold(), Sound->GetMaxPlayThreshold()
		);
		PlayCountAndThreshold.X = 0;
		CurrentSound = Sound;
	}

	// In tick, adjust volume based on distance to camera, until sound has completed

// VOLUME HANDLING

	void UPlaySoundComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction){
		Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
		//if (Sound->IsDynamic()) SoundLocation = Owner->GetActorLocation();
		
		// Calculate sound volume from sound manager
	}