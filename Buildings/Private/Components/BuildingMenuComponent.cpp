#include "Components/BuildingMenuComponent.h"
#include "Engine/EngineTypes.h"
#include "GP3Team16/GP3Team16GameModeBase.h"
#include "TimerManager.h"
#include "Data/PriceDataAsset.h"

UBuildingMenuComponent::UBuildingMenuComponent(){
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
	bAutoActivate = true;
}

void UBuildingMenuComponent::Purchase(UPriceDataAsset* Purchasable)
{
	if (!Purchasables.Contains(Purchasable) || !Purchasable->CanBePurchased(GetGameMode()))
		return;

	if (Purchasable->TryPurchase(GetGameMode()))
	{
		PurchaseQueue.Add(Purchasable);
		StartNextTimerIfAble();
	}
}

void UBuildingMenuComponent::CancelCurrentPurchase()
{
	if (!PurchaseQueue.Num())
		return;

	GetWorld()->GetTimerManager().ClearTimer(PurchaseTimerHandle);
	PurchaseQueue[0]->Refund(GetGameMode());
	PurchaseQueue.RemoveAt(0);
	StartNextTimerIfAble();
}

void UBuildingMenuComponent::StartNextTimerIfAble()
{
	if (GetWorld()->GetTimerManager().IsTimerActive(PurchaseTimerHandle) || !PurchaseQueue.Num())
		return;

	GetWorld()->GetTimerManager().SetTimer(PurchaseTimerHandle, this, &UBuildingMenuComponent::FinishPurchase, PurchaseQueue[0]->GetBuildDuration(), false);
}

void UBuildingMenuComponent::FinishPurchase()
{
	check(PurchaseQueue.Num() > 0);

	//This tells the GameMode to spawn/finalize whatever we are purchasing
	GetGameMode()->PurchaseAsset(GetOwner(), PurchaseQueue[0]);

	PurchaseQueue.RemoveAt(0);
	GetWorld()->GetTimerManager().ClearTimer(PurchaseTimerHandle);
	StartNextTimerIfAble();
}

AGP3Team16GameModeBase* UBuildingMenuComponent::GetGameMode()
{
	return GameMode ? GameMode : GameMode = GetWorld()->GetAuthGameMode<AGP3Team16GameModeBase>();
}

float UBuildingMenuComponent::GetBuildProgress()
{
	if (!PurchaseQueue.Num())
		return 0.0f;

	return 1.0f - GetWorld()->GetTimerManager().GetTimerRemaining(PurchaseTimerHandle) / PurchaseQueue[0]->GetBuildDuration();
}