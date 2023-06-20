// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/InputBindableInterface.h"
#include "UnitSelectorComponent.generated.h"

#define TRACE_DISTANCE 100000.0f
#define OVERLAP_BOX_HEIGHT 400.0f

class UInputAction;
class UEnhancedInputComponent;
class ISelectableUnitInterface;
class ISelectorTargetInterface;

/* This component should be attached to the Player's pawn.
* It handles selection of units and buildings, and assigning them targets,
* through left- and right-clicking respectively.
* Selectable actors must implement the ISelectableUnitInterface
* Targetable actors must implement the ISelectorTargetInterface
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PLAYER_API UUnitSelectorComponent : public UActorComponent, public IInputBindableInterface
{
	GENERATED_BODY()

public:

	//Manually send in a list of units for selection.
	//Does not verify selection priorities or group selections.
	//Make sure you only pass in units that SHOULD be selected together.
	UFUNCTION(BlueprintCallable)
	void UnsafeManualSelectList(TArray<AActor*> Units);

	UFUNCTION(BlueprintCallable)
	void ClearSelectedUnits();

protected:

	TObjectPtr<APlayerController> PlayerController;

	//Determines how we distinguish between single-click selection
	//and click-and-drag selection; mouse move distance or mouse hold time.
	UPROPERTY(EditAnywhere)
	bool bGroupSelectFromMovementInsteadOfTime = true;

	//If click-and-drag selection is based on mouse hold time, this determines duration.
	UPROPERTY(EditAnywhere)
	float BoxDragHoldDuration = 0.2f;
	float BoxDragStartTime;

	//If click-and-drag selection is based on mouse move distance, this determines distance.
	UPROPERTY(EditAnywhere)
	float MouseDistanceForGroupSelect = 10.0f;

	//Are we currently click-and-dragging?
	UPROPERTY(BlueprintReadOnly)
	bool bIsDragging = false;

	//If single selecting does not immediately hit a unit, 
	//this determines the size of the overlap box that attempts to find
	//a nearby selectable unit.
	UPROPERTY(EditAnywhere)
	float SingleSelectionAreaSize = 100.0f;

	UPROPERTY(BlueprintReadOnly)
	FVector2D CurrentMousePosition;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> LeftClickInputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> RightClickInputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> DeleteInputAction;

	//The list of currently selected units
	TArray<TObjectPtr<ISelectableUnitInterface>> SelectedUnits;

	UPROPERTY(BlueprintReadOnly)
	FVector2D DragStartMousePosition;

	UPROPERTY(BlueprintReadOnly)
	FVector2D DragEndMousePosition;

	//Trace channel for selecting units
	UPROPERTY(EditAnywhere)
	TEnumAsByte<ECollisionChannel> TraceChannelSelect;

	//Trace channel for selecting buildings
	UPROPERTY(EditAnywhere)
	TEnumAsByte<ECollisionChannel> TraceChannelSelectBuildings;

	//Trace channel for targeting by right-click
	UPROPERTY(EditAnywhere)
	TEnumAsByte<ECollisionChannel> TraceChannelTarget = ECollisionChannel::ECC_GameTraceChannel2;
	

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	//Methods called by input binding actions
	void RightClick();
	void LeftClickDragStart();
	void LeftClickDragStop();

	//Performs selections based on LeftClick input, 
	//SingleSelect for single click selection
	//GroupSelect for click-and-drag selection
	void SingleSelect();
	TArray<TObjectPtr<ISelectableUnitInterface>> GroupSelect();

	//From a list of selectable units, selects the units with the
	//highest priority. If any of the high-priority units are not group-selectable, 
	//only that unit is selected.
	void PrioritySelect(TArray<TObjectPtr<ISelectableUnitInterface>> SelectableUnits);

	//Deletes all selected units
	void DeleteSelected();

	//Using the PlayerController, traces a line under the mouse cursor to find
	//an actor implementing ISelectableUnitInterface.
	//If no actor is immediately hit, performs a Box Overlap on the ground
	//hit point to find a nearby actor instead, allowing for better
	//input accuracy.
	TObjectPtr<ISelectableUnitInterface> FindSelectableUnitUnderMouseCursor();

public:	
	// Sets default values for this component's properties
	UUnitSelectorComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//For IInputBindableInterface implementation
	void BindInput(UEnhancedInputComponent* InputComponent) override;
	void UnbindInput(UEnhancedInputComponent* InputComponent) override;
		
};
