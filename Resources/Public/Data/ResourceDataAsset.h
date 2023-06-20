#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Data/ResourceType.h"
#include "ResourceDataAsset.generated.h"

class AResourceActor;
class UPaperSprite;
enum class EResourceType : uint8;

UCLASS(BlueprintType)
class RESOURCES_API UResourceDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	const EResourceType GetResourceType() const { return Type; }
	const TSubclassOf<AResourceActor> GetActor() const { return Actor; }
	
	const uint8 GetValueInPile(const int difficulty) const {
		if (difficulty >= 0 && difficulty < ValueInPile.Num())
			return ValueInPile[difficulty];

		return 0;
	}
	const float GetExtractionTime() const { return ExtractionTime; }
	
	const UPaperSprite* GetIcon() const {
		if (Icon) return Icon.Get();
		return nullptr;
	};
	const FString& GetTooltip() const { return Tooltip; };

	const USoundWave* GetGatherSound() const { return GatherSound; }

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EResourceType Type = EResourceType::RT_UNSET;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AResourceActor> Actor = nullptr;

	
	UPROPERTY(Category="Behaviour", EditDefaultsOnly, BlueprintReadOnly)
	TArray<uint8> ValueInPile = { 4, 2, 1 };

	UPROPERTY(Category="Behaviour", EditDefaultsOnly, BlueprintReadOnly)
	float ExtractionTime = 1.5f;

	
	UPROPERTY(Category="Interface", EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UPaperSprite> Icon = nullptr;
	
	UPROPERTY(Category="Interface", EditDefaultsOnly, BlueprintReadOnly)
	FString Tooltip = "";

	UPROPERTY(Category="Sound", EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USoundWave> GatherSound = nullptr;

};