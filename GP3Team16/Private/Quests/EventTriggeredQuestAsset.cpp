// Fill out your copyright notice in the Description page of Project Settings.


#include "Quests/EventTriggeredQuestAsset.h"

void UEventTriggeredQuestAsset::TickWhileActive(float DeltaTime)
{
	BlueprintTick(DeltaTime);
}

void UEventTriggeredQuestAsset::SetQuestText(FString NewText)
{
	RuntimeQuestText = NewText;
	OnQuestTextChange.Broadcast();
}

void UEventTriggeredQuestAsset::SetActive(bool NewState, AGP3Team16GameModeBase* GameMode)
{
	if (!NewState)
	{
		CompletedSuccesses = 0;
		RuntimeQuestText.Empty();
	}
	Super::SetActive(NewState, GameMode);
}

void UEventTriggeredQuestAsset::CompleteQuest()
{
	if (bCanOnlyBeCompletedWhileActive && !bIsActive) return;

	CompletedSuccesses++;
	if (CompletedSuccesses == RequiredSuccesses)
	{
		bIsCompleted = true;
		OnQuestCompleted.Broadcast();
	}
	else 
	{
		OnQuestTextChange.Broadcast();
	}
}

FString UEventTriggeredQuestAsset::GetQuestText()
{
	FString SelectText = RuntimeQuestText.IsEmpty() ? QuestText : RuntimeQuestText;
	FString ReplaceFrom = FString(TEXT("[]"));
	FString ReplaceWith = FString::FromInt(RequiredSuccesses - CompletedSuccesses);
	FString ReturnText = QuestText.Replace((TCHAR*)*ReplaceFrom, (TCHAR*)*ReplaceWith, ESearchCase::IgnoreCase);
	return ReturnText;
}

void UEventTriggeredQuestAsset::ResetQuestState()
{
	Super::ResetQuestState();
	CompletedSuccesses = 0;
	RuntimeQuestText.Empty();
}