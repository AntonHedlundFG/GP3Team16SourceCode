#pragma once

#include "CoreMinimal.h"
#include "Components/AudioComponent.h"
#include "SuperAudioComponent.generated.h"

class USoundPriorityDataAsset;

UCLASS(ClassGroup="Audio", meta=(BlueprintSpawnableComponent))
class GP3TEAM16_API USuperAudioComponent : public UAudioComponent
{
	GENERATED_BODY()

public:

	USuperAudioComponent();
	
protected:

	UPROPERTY(Category="Super Audio", EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USoundPriorityDataAsset> SoundPriority = nullptr;

	UPROPERTY(Category="Super Audio|Time Thresholding", EditDefaultsOnly, BlueprintReadOnly)
	bool UseTimeThreshold = false;

	UPROPERTY(Category="Super Audio|Time Thresholding", EditDefaultsOnly, BlueprintReadOnly)
	float MinTimeBetweenPlay = 0.25f;

	
	UPROPERTY(Category="Super Audio|Count Thresholding", EditDefaultsOnly, BlueprintReadOnly)
	bool UseCountThreshold = false;
	
	UPROPERTY(Category="Super Audio|Count Thresholding", EditDefaultsOnly, BlueprintReadOnly)
	uint8 MinPlayCountThreshold = 3;

	UPROPERTY(Category="Super Audio|Count Thresholding", EditDefaultsOnly, BlueprintReadOnly)
	uint8 MaxPlayCountThreshold = 4;

	
	UPROPERTY(Category="Super Audio|Pitch Shifting", EditDefaultsOnly, BlueprintReadOnly)
	bool UsePitchShift = false;
	
	UPROPERTY(Category="Super Audio|Pitch Shifting", EditDefaultsOnly, BlueprintReadOnly)
	float MinPitchShift = 0.90f;
	
	UPROPERTY(Category="Super Audio|Pitch Shifting", EditDefaultsOnly, BlueprintReadOnly)
	float MaxPitchShift = 1.10f;
	
	
	virtual void BeginPlay() override;
	virtual void Play(float StartTime) override;

private:

	uint8 PlayCount = 0;
	uint8 PlayCountThreshold = 0;
	float PreviousPlayDateTime = 0;

	const bool HasExceededTime();
	const bool HasReachedCount();
	void SetPitchShift();
	
};
