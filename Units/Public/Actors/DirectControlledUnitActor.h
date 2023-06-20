// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Interfaces/SelectableUnitInterface.h"
#include "Delegates/Delegate.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DirectControlledUnitActor.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelectedStateChange, bool, State);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewWorldPositionTarget, FVector, Target);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewActorTarget, AActor*, Target);

UCLASS()
class UNITS_API ADirectControlledUnitActor : public ACharacter, public ISelectableUnitInterface
{
	GENERATED_BODY()
	
public:
	/*
	* This determines the selection priority of the unit.
	* Makes sure we can't select, for example, soldiers and buildings
	* at the same time. 
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EUnitSelectionPriority SelectionPriority;

	/*
	* This event is called whenever the unit is selected or 
	* unselected, passing along a bool with its new selection state
	*/
	UPROPERTY(BlueprintAssignable)
	FOnSelectedStateChange OnSelectedStateChange;

	/*
	* This event is called when the unit has received right-click
	* instructions without a valid target; which means it should 
	* move to the new world position.
	*/
	UPROPERTY(BlueprintAssignable)
	FOnNewWorldPositionTarget OnNewWorldPositionTarget;

	/*
	* This event is called when the unit has received right-click
	* instructions with a valid target; which means it should
	* interact with that target in whatever way is suitable.
	*/
	UPROPERTY(BlueprintAssignable)
	FOnNewActorTarget OnNewActorTarget;

	/*
	* This bitmask determines which targets are valid for right-click
	* instructions.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (Bitmask, BitmaskEnum = "/Script/Utilities.ESelectorTargetType"))
	uint8 SelectorTargets;

	/*
	* Determines if this unit can only be selected alone, or 
	* in a group with other units of the same priority.
	*/
	UPROPERTY(EditAnywhere)
	bool bIsGroupSelectable = true;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool bIsPrimarySelected = false;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bIsEnemyUnit = false;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Sets default values for this actor's properties
	ADirectControlledUnitActor();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//ISelectableUnitInterface virtual implementations
	EUnitSelectionPriority GetSelectionPriority() override;
	bool IsGroupSelectable() override;
	void SetSelected(bool bSelected) override;
	void AssignTarget(FVector TargetWorldPosition, TArray<TObjectPtr<AActor>> Targets) override;
	void SetPrimarySelected(bool bPrimarySelected) override;

};
