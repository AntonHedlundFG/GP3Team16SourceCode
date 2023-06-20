#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Buildings/Public/Actors/BuildingIndicator.h"
#include "PriceDataAsset.generated.h"

enum class EResourceType : uint8;
enum class EUnitStatType : uint8;
class AGP3Team16GameModeBase;
class UPaperSprite;
class UUnitStatsAsset;

UENUM(BlueprintType)
enum class EPurchasableType : uint8 {
	PT_Building		UMETA(DisplayName = "Building"),
	PT_Unit			UMETA(DisplayName = "Unit"),
	PT_Upgrade		UMETA(DisplayName = "Upgrade")
};

/* This Data Asset class contains information about a single
* kind of purchasable, for example "Worker Ant", 
* "Barracks Building" or "Tank Health Upgrade".
*/
UCLASS(BlueprintType)
class RESOURCES_API UPriceDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	//Attempts to pay the price for the component using the GameMode resources.
	//Returns true if successful.
	UFUNCTION(BlueprintCallable)
	bool TryPurchase(AGP3Team16GameModeBase* GameMode);

	//This method refunds the cost of a purchase
	// and sets "bHasBeenPurchased" to false.
	UFUNCTION(BlueprintCallable)
	void Refund(AGP3Team16GameModeBase* GameMode);

	UFUNCTION(BlueprintCallable)
	EPurchasableType GetType() const { return Type; }

	UFUNCTION(BlueprintCallable)
	TMap<EResourceType, int> GetPrices() { return Prices; }

	UFUNCTION(BlueprintCallable)
	FString GetName() { return ObjectName; }

	TSubclassOf<AActor> GetObjectAsset() const { return ObjectAsset; }
	TSubclassOf<ABuildingIndicator> GetIndicatorAsset() const { return IndicatorAsset; }
	
	UFUNCTION(BlueprintCallable)
	bool CanBePurchased(AGP3Team16GameModeBase* GameMode);
	
	UFUNCTION(BlueprintCallable)
	const float GetBuildDuration() const { return BuildDuration; }
	
	UFUNCTION(BlueprintCallable)
	FString GetTooltip() const { return Tooltip; }
	
	UFUNCTION(BlueprintCallable)
	UPaperSprite* GetIcon() const { return Icon; }

	UFUNCTION(BlueprintCallable)
	UUnitStatsAsset* GetUpgradedUnitAsset() { return UpgradeUnit; }

	UFUNCTION(BlueprintCallable)
	EUnitStatType GetUpgradeType() { return UpgradeType; }

	UFUNCTION(BlueprintCallable)
	bool IsAffordable(AGP3Team16GameModeBase* GameMode);
	
protected:

	UPROPERTY(Category = "All Assets", EditAnywhere)
	FString ObjectName;

	UPROPERTY(Category = "All Assets", EditDefaultsOnly)
	EPurchasableType Type;
	
	UPROPERTY(Category = "Buildings and Units", EditDefaultsOnly)
	TSubclassOf<AActor> ObjectAsset;

	UPROPERTY(Category = "Buildings", EditDefaultsOnly)
	TSubclassOf<ABuildingIndicator> IndicatorAsset;

	UPROPERTY(Category = "All Assets", EditDefaultsOnly)
	TMap<EResourceType, int> Prices;

	
	UPROPERTY(Category="All Assets", EditDefaultsOnly)
	float BuildDuration = 5.0f;

	UPROPERTY(Category="All Assets", EditDefaultsOnly)
	bool bPurchasableOnlyOnce = false;
	
	
	UPROPERTY(Category = "All Assets", EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UPaperSprite> Icon = nullptr;

	UPROPERTY(Category = "All Assets", EditDefaultsOnly, BlueprintReadOnly)
	FString Tooltip = "";

	UPROPERTY(Category = "Upgrades", EditAnywhere)
	TObjectPtr<UUnitStatsAsset> UpgradeUnit;

	UPROPERTY(Category = "Upgrades", EditAnywhere)
	EUnitStatType UpgradeType;

	UPROPERTY(Category = "All Assets", EditDefaultsOnly, BlueprintReadOnly)
	int EggAmountRequirement = 0;
	
	UPROPERTY(Transient)
	bool bHasBeenPurchased = false;
	
};
