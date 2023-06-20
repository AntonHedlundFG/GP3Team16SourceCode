// Fill out your copyright notice in the Description page of Project Settings.


#include "Quests/TimerQuestAsset.h"
#include "GP3Team16/GP3Team16GameModeBase.h"

void UTimerQuestAsset::SetActive(bool NewState, AGP3Team16GameModeBase* GameMode)
{
	if (NewState)
	{
		GameMode->GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UTimerQuestAsset::FinishQuestTimer, Duration, false);
	}
	Super::SetActive(NewState, GameMode);
}

void UTimerQuestAsset::FinishQuestTimer()
{
	bIsCompleted = true;
	OnQuestCompleted.Broadcast();
}