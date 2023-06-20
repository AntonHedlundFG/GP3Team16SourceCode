#include "GP3Team16GameModeBase.h"
#include "Actors/UpgradeManager.h"
#include "Actors/BuildingSpawnManager.h"
#include "Actors/UnitSpawnManager.h"
#include "Data/PriceDataAsset.h"
#include "Sound/SoundManager.h"

// INITIALISATION

	void AGP3Team16GameModeBase::BeginPlay(){
		Super::BeginPlay();
		UWorld* World = GetWorld();
		
		UnitSpawnManager = Cast<AUnitSpawnManager>(
			World->SpawnActor(AUnitSpawnManager::StaticClass())
		);

		BuildingSpawnManager = Cast<ABuildingSpawnManager>(
			World->SpawnActor(ABuildingSpawnManager::StaticClass())
		);

		UpgradeManager = Cast<AUpgradeManager>(
			World->SpawnActor(AUpgradeManager::StaticClass())
		);

		SoundManager = Cast<ASoundManager>(
			World->SpawnActor(ASoundManager::StaticClass())
		);
	}

// RESOURCE HANDLING

	int AGP3Team16GameModeBase::ResourceAmount(EResourceType Type){
		const int& IntRef = GatheredResources.FindOrAdd(Type);
		return IntRef;
	}

	bool AGP3Team16GameModeBase::TryModifyResourceAmount(EResourceType Type, int Modifier, bool Force){
		const int TargetValue = ResourceAmount(Type) + Modifier;
	
		if (!Force && TargetValue < 0)
			return false;
		
		GatheredResources[Type] = TargetValue;
		OnResourceChange.Broadcast(Type, TargetValue);
		return true;
	}

// PURCHASE HANDLING

	void AGP3Team16GameModeBase::PurchaseAsset(AActor* Building, UPriceDataAsset* Purchase){
		switch (Purchase->GetType()){
		case EPurchasableType::PT_Unit:
				OnPurchaseUnit.ExecuteIfBound(Building, Purchase);
				break;
			case EPurchasableType::PT_Building:
				OnPurchaseBuilding.ExecuteIfBound(Building, Purchase);
				break;
			case EPurchasableType::PT_Upgrade:
				OnPurchaseUpgrade.ExecuteIfBound(Building, Purchase);
				break;
		}
	}
	