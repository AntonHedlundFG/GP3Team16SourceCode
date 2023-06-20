#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Data/ResourceType.h"
#include "Components/UnitSelectorComponent.h"
#include "GP3Team16GameModeBase.generated.h"

class ABuildingSpawnManager;
class UPriceDataAsset;
class AUnitSpawnManager;
class AUpgradeManager;
class ASoundManager;
class UUnitSelectorComponent;
enum class EResourceType : uint8;

DECLARE_DELEGATE_TwoParams(FOnPurchaseDelegate, AActor*, UPriceDataAsset*);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnResourceChangeDelegate, EResourceType, Type, int, Amount);

UCLASS()
class GP3TEAM16_API AGP3Team16GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	FOnPurchaseDelegate OnPurchaseUnit;
	FOnPurchaseDelegate OnPurchaseBuilding;
	FOnPurchaseDelegate OnPurchaseUpgrade;

	UPROPERTY(BlueprintAssignable)
	FOnResourceChangeDelegate OnResourceChange;
	
	
	UFUNCTION(BlueprintCallable)
	int ResourceAmount(EResourceType Type);

	//Returns false if you try to go below 0, which fails.
	UFUNCTION(BlueprintCallable)
	bool TryModifyResourceAmount(EResourceType Type, int Modifier, bool Force = false);
	
	UFUNCTION(BlueprintCallable)
	void PurchaseAsset(AActor* Building, UPriceDataAsset* Purchase);


	//This can be used to register an Ant actor in the game mode.
	//If registering an actor, make sure to DeRegisterUnit() it once removed
	UFUNCTION(BlueprintCallable)
	void RegisterUnit(AActor* Unit) { AntsInWorld.AddUnique(Unit); }

	//Removes a unit previously registered using RegisterUnit()
	UFUNCTION(BlueprintCallable)
	void DeRegisterUnit(AActor* Unit) { AntsInWorld.Remove(Unit); }

	UFUNCTION(BlueprintCallable)
	void RegisterUnitSelectorComponent(UUnitSelectorComponent* Selector) 
	{ UnitSelectorComponent = Selector; }

	UFUNCTION(BlueprintCallable)
	UUnitSelectorComponent* GetUnitSelectorComponent() 
	{ return UnitSelectorComponent; }

	UFUNCTION(BlueprintCallable)
	void ClearSelectedUnits() { if(UnitSelectorComponent) UnitSelectorComponent->ClearSelectedUnits(); }

	//Retreives a list of all actors registered with RegisterUnit().
	UFUNCTION(BlueprintCallable)
	TArray<AActor*> GetRegisteredUnits() { return AntsInWorld; }

	UFUNCTION(BlueprintCallable)
	AUnitSpawnManager* GetUnitSpawnManager() const { return UnitSpawnManager; }

	UFUNCTION(BlueprintCallable)
	ABuildingSpawnManager* GetBuildingSpawnManager() const { return BuildingSpawnManager; }
	
	UFUNCTION(BlueprintCallable)
	AUpgradeManager* GetUpgradeManager() const { return UpgradeManager; }

	UFUNCTION(BlueprintCallable)
	ASoundManager* GetSoundManager() const { return SoundManager; }
	
protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	TMap<EResourceType, int> GatheredResources;

	//Stores a list of registered Ant actors
	TArray<AActor*> AntsInWorld;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AUnitSpawnManager> UnitSpawnManager;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ABuildingSpawnManager> BuildingSpawnManager;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AUpgradeManager> UpgradeManager;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ASoundManager> SoundManager;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UUnitSelectorComponent> UnitSelectorComponent;
};
