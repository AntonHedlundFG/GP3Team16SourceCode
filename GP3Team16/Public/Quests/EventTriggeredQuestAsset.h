// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Quests/QuestBaseAsset.h"
#include "EventTriggeredQuestAsset.generated.h"

/** This is a quest designed to be completed by making "CompleteQuest()"
 * calls from blueprints. 
 * It is fully completed when CompleteQuest has been called a number of times
 * equal to RequiredSuccesses.
 */
UCLASS(BlueprintType)
class GP3TEAM16_API UEventTriggeredQuestAsset : public UQuestBaseAsset
{
	GENERATED_BODY()

public:
	//UQuestBaseAsset overrides
	virtual void SetActive(bool NewState, AGP3Team16GameModeBase* GameMode) override;
	FString GetQuestText() override;
	void ResetQuestState() override;
	void TickWhileActive(float DeltaTime) override;
	
	//Passes along the TickWhileActive calls to blueprints
	UFUNCTION(BlueprintImplementableEvent)
	void BlueprintTick(float DeltaTime);

	//Can be called from blueprints to set new quest text in runtime.
	UFUNCTION(BlueprintCallable)
	void SetQuestText(FString NewText);

	//Call this method from blueprints to attempt quest completion.
	UFUNCTION(BlueprintCallable)
	void CompleteQuest();


protected:
	//Used so we don't overwrite the non-transient default text
	UPROPERTY(Transient)
	FString RuntimeQuestText;

	//How many times does CompleteQuest() have to be called to complete?
	UPROPERTY(EditAnywhere)
	int RequiredSuccesses = 1;

	//How many times have CompleteQuest() been called?
	UPROPERTY(Transient)
	int CompletedSuccesses = 0;
	
};
