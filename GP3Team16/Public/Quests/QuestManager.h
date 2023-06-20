// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuestManager.generated.h"

class UQuestBaseAsset;
class AGP3Team16GameModeBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUITextChange, FString, Text, bool, bIsNewQuest);

/* This actor can be placed in a scene to activate the quest system.
* It stores a list of Quest data assets (QuestBaseAsset.h)
* And runs through them one by one, in order, to completion.
*/
UCLASS()
class GP3TEAM16_API AQuestManager : public AActor
{
	GENERATED_BODY()
	
public:

	//This is the list of all quests that should be used, in order.
	UPROPERTY(EditAnywhere)
	TArray<UQuestBaseAsset*> QuestList;

	//This can be called to skip all quests, which disables the quest system,
	//while still making sure all quests are marked as completed
	//so all systems depending on quest completion are still activated.
	UFUNCTION(BlueprintCallable)
	void SkipQuests();
	
	//This is the delegate meant to connect the Quest Manager to the UI
	//Will broadcast every time there's an update to the quest text.
	//The second parameter is true if the new quest text belongs to a 
	//new quest rather than an update to an existing one.
	//If an empty string is broadcast, all quests are done and the UI 
	//can be disabled.
	UPROPERTY(BlueprintAssignable)
	FOnUITextChange OnUITextChange;

protected:
	UPROPERTY(VisibleAnywhere)
	int CurrentQuestIndex = -1;

	//This is run in BeginPlay. It resets the state of all quests
	//and starts the first quest.
	void InitQuestList();

	//This broadcasts the OnUITextChange delegate.
	UFUNCTION()
	void UpdateUI();

	//This moves on to the next quest, removing existing delegate references
	//and setting up new ones. It also checks if we've finished all quests
	UFUNCTION()
	void AdvanceQuest();
	
	//Used by AdvanceQuest() by setting a short timer, so that quest completion
	//isn't instant, improving the feel.
	void StartNextQuest();
	FTimerHandle NextQuestDelayHandle;

	//Simply broadcasts an empty string to OnUITextChange, which informs
	//subscribers to disable
	void DisableUI();

	//Called each Tick, passes along tick calls to the current quest.
	void TickActiveQuest(float DeltaTime);
	
	TObjectPtr<AGP3Team16GameModeBase> GameMode;

	//Keeps track of whether a new quest has started, which we use for the
	//second parameter of the OnUITextChange broadcast.
	bool bNewQuestActive = true;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Sets default values for this actor's properties
	AQuestManager();
	// Called every frame
	virtual void Tick(float DeltaTime) override;



};
