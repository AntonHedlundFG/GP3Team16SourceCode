// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SelectableUnitInterface.generated.h"


UENUM()
enum class EUnitSelectionPriority : uint8 {
	USP_SOLDIER = 0		UMETA(DisplayName = "Soldier"),
	USP_WORKER = 1		UMETA(DisplayName = "Worker"),
	USP_SCOUT = 2		UMETA(DisplayName = "Scout"),
	USP_BUILDING = 3	UMETA(DisplayName = "Building")
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class USelectableUnitInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UTILITIES_API ISelectableUnitInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/*
	* Returns the selection priorty of the unit. 
	* When multiple units are marked for selection, only the lowest priority values are selected.
	*/
	virtual EUnitSelectionPriority GetSelectionPriority() = 0;

	/*
	* Returns false if the unit can ONLY be selected on its own
	* This is likely only going to be true for buildings
	*/
	virtual bool IsGroupSelectable() = 0;

	/*
	* Called by the UnitSelectorComponent to tell it it is selected
	* mostly for visual purposes.
	*/
	virtual void SetSelected(bool bSelected) = 0;
	
	/*
	* This is used to determine if a unit should play selection sounds
	*/
	virtual void SetPrimarySelected(bool bPrimarySelected) = 0;

	/*
	* Method used to tell a selected unit to move to / attack
	* a target. Pass the blocking hits worldposition, and a list of possible
	* targets, so that the unit itself can choose which actor to actually target
	*/
	virtual void AssignTarget(FVector TargetWorldPosition, TArray<TObjectPtr<AActor>> Targets) = 0;

	UFUNCTION(BlueprintImplementableEvent)
	bool IsDeletable();
};
