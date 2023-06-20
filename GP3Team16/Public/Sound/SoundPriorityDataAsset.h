#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SoundPriority.h"
#include "SoundPriorityDataAsset.generated.h"

UCLASS()
class GP3TEAM16_API USoundPriorityDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	const ESoundPriority GetSoundPriority() const { return SoundPriority; }
	const float GetVolumeMultiplier() const { return VolumeMultiplier; }

	const bool AllowsSpatialisation() const { return AllowSpatialisation; }
	USoundAttenuation* GetAttenuationSettings() const { return AttenuationSettings; } 
		
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<ESoundPriority> SoundPriority = SP_UNSPECIFIED;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float VolumeMultiplier = 1;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool AllowSpatialisation = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USoundAttenuation> AttenuationSettings = nullptr;
	
};
