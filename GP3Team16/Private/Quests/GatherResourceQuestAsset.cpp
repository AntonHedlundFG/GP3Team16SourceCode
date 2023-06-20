// Fill out your copyright notice in the Description page of Project Settings.


#include "Quests/GatherResourceQuestAsset.h"
#include "GP3Team16/GP3Team16GameModeBase.h"

void UGatherResourceQuestAsset::SetActive(bool NewState, AGP3Team16GameModeBase* GameMode)
{

	if (GameMode)
	{
		if (NewState)
		{
			GatheredAmount = GameMode->ResourceAmount(QuestResourceType);
			GatheredWhileActive = 0;
			GameMode->OnResourceChange.AddDynamic(this, &UGatherResourceQuestAsset::ResourceGathered);
		}
		else
		{
			GameMode->OnResourceChange.RemoveDynamic(this, &UGatherResourceQuestAsset::ResourceGathered);
			GatheredAmount = 0;
			GatheredWhileActive = 0;
		}
	}
	
	Super::SetActive(NewState, GameMode);

}

void UGatherResourceQuestAsset::ResourceGathered(EResourceType Type, int Amount)
{
	if (Type != QuestResourceType) return;

	if (bCanOnlyBeCompletedWhileActive)
	{
		//This happens if resources are spent rather than gathered.
		if (Amount < GatheredAmount)
		{
			GatheredAmount = Amount;
			return;
		}

		GatheredWhileActive += Amount - GatheredAmount;
		GatheredAmount = Amount;

		if (GatheredWhileActive >= TargetAmount)
		{
			bIsCompleted = true;
			OnQuestCompleted.Broadcast();
		}
		else
		{
			OnQuestTextChange.Broadcast();
		}
	}
	else
	{
		if (Amount >= TargetAmount)
		{
			bIsCompleted = true;
			OnQuestCompleted.Broadcast();
		}
		else
		{
			GatheredAmount = Amount;
			OnQuestTextChange.Broadcast();
		}
	}
	
}

FString UGatherResourceQuestAsset::GetQuestText()
{
	FString ReplaceFrom = FString(TEXT("[]"));
	FString ReplaceWith = bCanOnlyBeCompletedWhileActive
		? FString::FromInt(TargetAmount - GatheredWhileActive)
		: FString::FromInt(TargetAmount - GatheredAmount);
	FString ReturnText = QuestText.Replace((TCHAR*)*ReplaceFrom, (TCHAR*)*ReplaceWith, ESearchCase::IgnoreCase);
	return ReturnText;
}

void UGatherResourceQuestAsset::ResetQuestState()
{
	Super::ResetQuestState();
	GatheredAmount = 0;
	GatheredWhileActive = 0;
}