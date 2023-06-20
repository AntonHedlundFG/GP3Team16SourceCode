// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/PriceDataAsset.h"
#include "GP3Team16/GP3Team16GameModeBase.h"

bool UPriceDataAsset::IsAffordable(AGP3Team16GameModeBase* GameMode)
{
	for (TPair<EResourceType, int> Element : Prices)
	{
		if (GameMode->ResourceAmount(Element.Key) < Element.Value)
			return false;
	}
	return true;
}

bool UPriceDataAsset::TryPurchase(AGP3Team16GameModeBase* GameMode)
{
	if (!CanBePurchased(GameMode) || !IsAffordable(GameMode))
		return false;

	for (const TPair<EResourceType, int> Element : Prices)
	{
		GameMode->TryModifyResourceAmount(Element.Key, -Element.Value);
	}
	bHasBeenPurchased = true;
	return true;
}

void UPriceDataAsset::Refund(AGP3Team16GameModeBase* GameMode)
{
	for (const TPair<EResourceType, int> Element : Prices)
	{
		GameMode->TryModifyResourceAmount(Element.Key, Element.Value);
	}
	bHasBeenPurchased = false;
}

bool UPriceDataAsset::CanBePurchased(AGP3Team16GameModeBase* GameMode)
{
	return (!bPurchasableOnlyOnce || !bHasBeenPurchased) 
		&& (GameMode->ResourceAmount(EResourceType::RT_EGG) >= EggAmountRequirement);
}