#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BuildingMenuComponent.generated.h"

class UPriceDataAsset;
class AGP3Team16GameModeBase;
struct FTimerHandle;

/* This is a component you can attach to a building to manage
* a list of Purchasables (stored as list of UPriceDataAsset.) 
* It is intended to be used to generate a UI where the player
* can select what to buy.
*/ 
UCLASS(ClassGroup="Buildings", meta=(BlueprintSpawnableComponent))
class BUILDINGS_API UBuildingMenuComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UBuildingMenuComponent();

	//This is the list of all purchasables you can buy in this building
	UFUNCTION(BlueprintCallable)
	const TArray<UPriceDataAsset*>& GetPurchasables() const { return Purchasables; }

	//Attempts to pay for a purchase,
	//and adds it to the buildings purchase queue if able to pay
	UFUNCTION(BlueprintCallable)
	void Purchase(UPriceDataAsset* Purchasable);

	//Cancels the first purchase in the queue and refunds its cost.
	UFUNCTION(BlueprintCallable)
	void CancelCurrentPurchase();

	//Returns the current purchase queue, only index 0 is being built
	UFUNCTION(BlueprintCallable)
	TArray<UPriceDataAsset*> GetPurchaseQueue() { return PurchaseQueue; }

	//Returns the current purchase progress as a percentage, from 0.0f to 1.0f.
	UFUNCTION(BlueprintCallable)
	float GetBuildProgress();
	
protected:

	//This is the list of all purchasables you can buy in this building
	UPROPERTY(Category="Buildings", EditDefaultsOnly, BlueprintReadOnly)
	TArray<UPriceDataAsset*> Purchasables;

	//A list of all queued purchases in this building. Only one 
	//purchase is processed at a time (index 0)
	UPROPERTY(BlueprintReadOnly)
	TArray<UPriceDataAsset*> PurchaseQueue;

	//A timer handle used to keep track of the purchase at the front 
	//of the queue
	UPROPERTY(VisibleAnywhere)
	FTimerHandle PurchaseTimerHandle;

	//Call this any time a change is made to the PurchaseQueue
	//To start the next purchase in the queue
	void StartNextTimerIfAble();

	//This is called by PurchaseTimerHandle when the purchase
	//at the front of the queue is finished processing. 
	//It notifies the Game Mode about the purchase.
	void FinishPurchase();

	//We don't fetch this in BeginPlay, we fetch in runtime 
	//the first time the method is called.
	AGP3Team16GameModeBase* GetGameMode();
	TObjectPtr<AGP3Team16GameModeBase> GameMode;

};
