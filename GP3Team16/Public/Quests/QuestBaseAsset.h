// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "QuestBaseAsset.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStateChange, bool, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuestTextChange);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuestCompleted);

class AGP3Team16GameModeBase;

/* This is the base class for all Quest Data Assets. 
*/
UCLASS(BlueprintType)
class GP3TEAM16_API UQuestBaseAsset : public UDataAsset
{
	GENERATED_BODY()
	

public:
	//Is this quest completed?
	UFUNCTION(BlueprintCallable)
	bool IsCompleted() { return bIsCompleted; }

	//This tick function is called by the QuestManager every tick
	//but only while the quest is active.
	virtual void TickWhileActive(float DeltaTime) { return; }

	//This contains the base functionality required to activate and
	//de-activate a quest. If overridden, make sure to call Super::SetActive
	UFUNCTION(BlueprintCallable)
		virtual void SetActive(bool NewState, AGP3Team16GameModeBase* GameMode) {
			bIsActive = NewState;
			OnStateChange.Broadcast(NewState);
			OnQuestTextChange.Broadcast();
			if (!bIsActive) { bIsCompleted = false; }
		}
	
	//Is this the active quest?
	UFUNCTION(BlueprintCallable)
	bool IsActive() const { return bIsActive; }

	//This delegate is broadcast whenever the "IsActive()" state changes.
	UPROPERTY(BlueprintAssignable)
	FOnStateChange OnStateChange;

	//This delegate is broadcast whenever the "GetQuestText() string changes.
	UPROPERTY(BlueprintAssignable)
	FOnQuestTextChange OnQuestTextChange;

	//This delegate is broadcast whenever the "IsCompleted()" state changes.
	UPROPERTY(BlueprintAssignable)
	FOnQuestCompleted OnQuestCompleted;

	//Gets the current quest text description. This text can change during 
	//quest completion. Register to the OnQuestCompleted delegate to 
	//be notified of changes.
	UFUNCTION(BlueprintCallable)
	virtual FString GetQuestText() { return QuestText; }

	//This contains the base functinoality required to reset a quest.
	//If overridden, make sure to call Super::ResetQuestState
	UFUNCTION(BlueprintCallable)
	virtual void ResetQuestState() { bIsActive = false; bIsCompleted = false; }

protected:
	//Is this the active quest?
	UPROPERTY(Transient, VisibleAnywhere)
	bool bIsActive = false;

	//Is this quest completed?
	UPROPERTY(Transient, VisibleAnywhere)
	bool bIsCompleted = false;

	//Can this quest be completed only while active?
	//If false, the quest can be completed before its activation.
	UPROPERTY(EditAnywhere)
	bool bCanOnlyBeCompletedWhileActive = false;

	// This text will be shown in the quest log. 
	// If the quest has values (for example "collect 5 food"), you can write
	// [] in the text to insert the value.
	// For example: "You need food! Collect [] food by.."
	UPROPERTY(EditAnywhere)
	FString QuestText;

};
