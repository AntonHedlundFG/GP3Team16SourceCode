#include "Actors/UpgradeManager.h"
#include "Data/PriceDataAsset.h"
#include "Data/UnitStatsAsset.h"
#include "Kismet/GameplayStatics.h"
#include "GP3Team16/GP3Team16GameModeBase.h"

// Sets default values
AUpgradeManager::AUpgradeManager()
{
 	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AUpgradeManager::BeginPlay()
{
	Super::BeginPlay();
	
	//Hooks up our purchase function to the OnPurchaseUpgrade delegate in the GameMode.
	if (GetGameMode())
		GetGameMode()->OnPurchaseUpgrade.BindUObject(this, &AUpgradeManager::PurchaseUpgrade);
}

AGP3Team16GameModeBase* AUpgradeManager::GetGameMode()
{
	if (!GameMode)
		GameMode = Cast<AGP3Team16GameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	return GameMode;
}

void AUpgradeManager::PurchaseUpgrade(AActor* PurchaseSource, UPriceDataAsset* PriceData)
{
	check(PriceData->GetType() == EPurchasableType::PT_Upgrade);

	PurchasedUpgrades.AddUnique(PriceData);

	//Here we check which units data asset we should upgrade, and which 
	//of its stats should be upgraded. The actual values are set in the
	//units data asset itself.
	UUnitStatsAsset* UpgradeAsset = PriceData->GetUpgradedUnitAsset();
	switch (PriceData->GetUpgradeType()) 
	{
		case EUnitStatType::UST_Damage:
			PriceData->GetUpgradedUnitAsset()->DamageTier++;
			break;
		case EUnitStatType::UST_Range:
			PriceData->GetUpgradedUnitAsset()->RangeTier++;
			break;
		case EUnitStatType::UST_MoveSpeed:
			PriceData->GetUpgradedUnitAsset()->MoveSpeedTier++;
			break;
		case EUnitStatType::UST_MaxHealth:
			PriceData->GetUpgradedUnitAsset()->MaxHealthTier++;
			break;
	}
}