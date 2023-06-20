// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Quests/QuestBaseAsset.h"
#include "Data/ResourceType.h"
#include "GatherResourceQuestAsset.generated.h"



/** This is a quest that keeps track of resources gathered in the GameMode
 */
UCLASS(BlueprintType)
class GP3TEAM16_API UGatherResourceQuestAsset : public UQuestBaseAsset
{
	GENERATED_BODY()

public:
	//UQuestBaseAsset overrides
	void SetActive(bool NewState, AGP3Team16GameModeBase* GameMode) override;
	FString GetQuestText() override;
	void ResetQuestState() override;
	
protected:
	//Determines which resource the quest is tracking
	UPROPERTY(EditAnywhere)
	EResourceType QuestResourceType;

	//Determines the target amount of resources to be gathered.
	//If bCanOnlyBeCompletedWhileActive is true it only counts 
	//resources added while quest is active.
	//If it is false, the target amount is a required TOTAL amount
	UPROPERTY(EditAnywhere)
	int TargetAmount = 1;

	//This is used if bCanOnlyBeCompletedWhileActive to keep track
	//of the current amount of gathered resources.
	UPROPERTY(Transient)
	int GatheredAmount;

	//This is used if bCanOnlyBeCompletedWhileActive to keep track
	//of how many resources have been gathered during the quest being active
	UPROPERTY(Transient)
	int GatheredWhileActive;

	//This function is bound to the OnResourceChange delegate of
	//the GameMode to receive changes in resource amounts.
	UFUNCTION()
	void ResourceGathered(EResourceType Type, int Amount);
};
