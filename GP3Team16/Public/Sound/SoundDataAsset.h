#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SoundPriority.h"
#include "SoundDataAsset.generated.h"

class USoundBase;

UCLASS(BlueprintType)
class GP3TEAM16_API USoundDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	const ESoundPriority GetPriority() const { return Priority; }
	USoundBase* GetSound() const { return Sound; }
	
	const bool IsDynamic() const { return Dynamic; }
	const uint8 GetMinPlayThreshold() const { return MinPlayThreshold; }
	const uint8 GetMaxPlayThreshold() const { return MaxPlayThreshold; }
	const float GetMinPitchShift() const { return MinPitchShift; }
	const float GetMaxPitchShift() const { return MinPitchShift; }
	const float GetDefaultVolume() const { return DefaultVolume; }

protected:

	UPROPERTY(Category="Sound", EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<ESoundPriority> Priority = SP_UNSPECIFIED;
	
	UPROPERTY(Category="Sound", EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USoundBase> Sound = nullptr;

	
	UPROPERTY(Category="Settings", EditDefaultsOnly, BlueprintReadOnly)
	bool Dynamic = false;
	
	UPROPERTY(Category="Settings", EditDefaultsOnly, BlueprintReadOnly)
	uint8 MinPlayThreshold = 3;

	UPROPERTY(Category="Settings", EditDefaultsOnly, BlueprintReadOnly)
	uint8 MaxPlayThreshold = 4;
	
	UPROPERTY(Category="Settings", EditDefaultsOnly, BlueprintReadOnly)
	float MinPitchShift = 1.0f;

	UPROPERTY(Category="Settings", EditDefaultsOnly, BlueprintReadOnly)
	float MaxPitchShift = 1.0f;

	UPROPERTY(Category="Settings", EditDefaultsOnly, BlueprintReadOnly)
	float DefaultVolume = 1.0f;
	
};
