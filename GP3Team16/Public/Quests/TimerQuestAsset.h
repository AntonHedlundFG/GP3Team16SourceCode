// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Quests/QuestBaseAsset.h"
#include "TimerQuestAsset.generated.h"

class AGP3Team16GameModeBase;
/** This is a quest that, once activated, automatically completes
 * after a set amount of time.
 */
UCLASS()
class GP3TEAM16_API UTimerQuestAsset : public UQuestBaseAsset
{
	GENERATED_BODY()

public:
	virtual void SetActive(bool NewState, AGP3Team16GameModeBase* GameMode) override;

protected:
	//How long should the quest be active before automatic completion?
	UPROPERTY(EditAnywhere)
	float Duration = 8.0f;

	UFUNCTION()
	void FinishQuestTimer();
	FTimerHandle TimerHandle;
};
