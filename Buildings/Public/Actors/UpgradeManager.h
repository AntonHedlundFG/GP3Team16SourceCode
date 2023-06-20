#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UpgradeManager.generated.h"

enum class EUnitStatType : uint8;
class UPriceDataAsset;
class AGP3Team16GameModeBase;

/* This actor is to be spawned by the GameMode to manage upgrade purchases.
*/

UCLASS()
class BUILDINGS_API AUpgradeManager : public AActor
{
	GENERATED_BODY()
	
public:
	AUpgradeManager();
	
	//Actually performs an upgrade purchase. Assumes payment has already been made.
	void PurchaseUpgrade(AActor* PurchaseSource, UPriceDataAsset* PriceData);

	//Checks if a specific upgrade has already been purchased.
	bool HasPurchasedUpgrade(UPriceDataAsset* PriceData)
	{
		return PurchasedUpgrades.Contains(PriceData);
	}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//A list of all already finished upgrade purchases.
	TArray<UPriceDataAsset*> PurchasedUpgrades;

	//We fetch this reference the first time the method is called.
	AGP3Team16GameModeBase* GetGameMode();
	TObjectPtr<AGP3Team16GameModeBase> GameMode;

	

};
