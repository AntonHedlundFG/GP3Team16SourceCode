// Fill out your copyright notice in the Description page of Project Settings.


#include "Quests/QuestManager.h"
#include "Quests/QuestBaseAsset.h"
#include "Kismet/GameplayStatics.h"
#include "GP3Team16/GP3Team16GameModeBase.h"

// Sets default values
AQuestManager::AQuestManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AQuestManager::BeginPlay()
{
	Super::BeginPlay();
	AGameModeBase* GameModeBase = UGameplayStatics::GetGameMode(this);
	GameMode = Cast<AGP3Team16GameModeBase>(GameModeBase);
	InitQuestList();
}

// Called every frame
void AQuestManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	TickActiveQuest(DeltaTime);
}

void AQuestManager::TickActiveQuest(float DeltaTime)
{
	if (CurrentQuestIndex >= 0 && CurrentQuestIndex < QuestList.Num())
		QuestList[CurrentQuestIndex]->TickWhileActive(DeltaTime);
}

void AQuestManager::InitQuestList()
{
	if (QuestList.Num() == 0) return;

	CurrentQuestIndex++;
	UQuestBaseAsset* CurrentQuest = QuestList[CurrentQuestIndex];
	if (!CurrentQuest)
	{
		return;
	}
	for (UQuestBaseAsset* Quest : QuestList)
	{
		Quest->ResetQuestState();
	}
	CurrentQuest->OnQuestCompleted.AddDynamic(this, &AQuestManager::AdvanceQuest);
	CurrentQuest->OnQuestTextChange.AddDynamic(this, &AQuestManager::UpdateUI);
	CurrentQuest->SetActive(true, GameMode);
}

void AQuestManager::UpdateUI()
{
	OnUITextChange.Broadcast(QuestList[CurrentQuestIndex]->GetQuestText(), bNewQuestActive);
	bNewQuestActive = false;
}
void AQuestManager::DisableUI()
{
	OnUITextChange.Broadcast(FString(""), true);
}

void AQuestManager::AdvanceQuest()
{
	if (!QuestList[CurrentQuestIndex]) return;
	QuestList[CurrentQuestIndex]->OnQuestCompleted.RemoveDynamic(this, &AQuestManager::AdvanceQuest);
	QuestList[CurrentQuestIndex]->OnQuestTextChange.RemoveDynamic(this, &AQuestManager::UpdateUI);
	while (CurrentQuestIndex < QuestList.Num() && QuestList[CurrentQuestIndex]->IsCompleted())
	{
		QuestList[CurrentQuestIndex]->SetActive(false, GameMode);
		CurrentQuestIndex++;
	}
	if (CurrentQuestIndex >= QuestList.Num()) 
	{
		DisableUI();
		return;
	}
	bNewQuestActive = true;
	GetWorld()->GetTimerManager().SetTimer(NextQuestDelayHandle, this, &AQuestManager::StartNextQuest, 0.5f, false);
}

void AQuestManager::StartNextQuest()
{
	QuestList[CurrentQuestIndex]->OnQuestCompleted.AddDynamic(this, &AQuestManager::AdvanceQuest);
	QuestList[CurrentQuestIndex]->OnQuestTextChange.AddDynamic(this, &AQuestManager::UpdateUI);
	QuestList[CurrentQuestIndex]->SetActive(true, GameMode);
}

void AQuestManager::SkipQuests()
{
	QuestList[CurrentQuestIndex]->OnQuestCompleted.RemoveDynamic(this, &AQuestManager::AdvanceQuest);
	QuestList[CurrentQuestIndex]->OnQuestTextChange.RemoveDynamic(this, &AQuestManager::UpdateUI);
	while (CurrentQuestIndex < QuestList.Num())
	{
		//Here we "dummy-complete" each quest so that any systems that are waiting for
		//specific quest completions are activated when we skip quests.
		QuestList[CurrentQuestIndex]->OnQuestCompleted.Broadcast();
		QuestList[CurrentQuestIndex]->OnStateChange.Broadcast(true);
		QuestList[CurrentQuestIndex]->OnStateChange.Broadcast(false);
		QuestList[CurrentQuestIndex]->ResetQuestState();
		CurrentQuestIndex++;
	}
	DisableUI();
}