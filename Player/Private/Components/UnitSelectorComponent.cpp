// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/UnitSelectorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Interfaces/SelectableUnitInterface.h"
#include "Interfaces/SelectorTargetInterface.h"
#include "GP3Team16/GP3Team16GameModeBase.h"

// Sets default values for this component's properties
UUnitSelectorComponent::UUnitSelectorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UUnitSelectorComponent::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	AGP3Team16GameModeBase* GameMode = Cast<AGP3Team16GameModeBase>(
		UGameplayStatics::GetGameMode(GetWorld())
		);
	if (GameMode)
		GameMode->RegisterUnitSelectorComponent(this);
	
}


// Called every frame
void UUnitSelectorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PlayerController)
	{
		PlayerController->GetMousePosition(CurrentMousePosition.X, CurrentMousePosition.Y);
	}

	//See if any of the selected units are now invalid; in which case remove them from the list
	//and assign the next one to be the primary unit.
	for (int i = SelectedUnits.Num() - 1; i >= 0; i--)
	{
		if (IsValid(SelectedUnits[i]->_getUObject()))
			continue;
		
		
		SelectedUnits.RemoveAt(i);

		if (!i && SelectedUnits.Num())
			SelectedUnits[0]->SetPrimarySelected(true);
	}

	// ...
}

void UUnitSelectorComponent::BindInput(UEnhancedInputComponent* InputComponent)
{
	if (!InputComponent || !LeftClickInputAction || !RightClickInputAction) return;

	InputComponent->BindAction(RightClickInputAction, ETriggerEvent::Completed, this, &UUnitSelectorComponent::RightClick);
	InputComponent->BindAction(LeftClickInputAction, ETriggerEvent::Triggered, this, &UUnitSelectorComponent::LeftClickDragStart);
	InputComponent->BindAction(LeftClickInputAction, ETriggerEvent::Completed, this, &UUnitSelectorComponent::LeftClickDragStop);
	InputComponent->BindAction(DeleteInputAction, ETriggerEvent::Completed, this, &UUnitSelectorComponent::DeleteSelected);
}

void UUnitSelectorComponent::UnbindInput(UEnhancedInputComponent* InputComponent)
{
	
}
void UUnitSelectorComponent::LeftClickDragStart()
{
	if (bIsDragging || !IsActive()) { return; }

	DragStartMousePosition = CurrentMousePosition;
	BoxDragStartTime = UGameplayStatics::GetRealTimeSeconds(this);
	bIsDragging = true;
}
void UUnitSelectorComponent::LeftClickDragStop()
{
	if (!bIsDragging) { return; }
	//When we stop leftclicking, if enough time has passed since we started, we will perform a group
	//selection, otherwise a single selection at mouse cursor location.
	bIsDragging = false;
	ClearSelectedUnits();
	DragEndMousePosition = CurrentMousePosition;
	
	if ((!bGroupSelectFromMovementInsteadOfTime && 
		UGameplayStatics::GetRealTimeSeconds(this) >= BoxDragStartTime + BoxDragHoldDuration)
		|| 
		(bGroupSelectFromMovementInsteadOfTime && 
		(DragStartMousePosition - DragEndMousePosition).GetAbsMax() > MouseDistanceForGroupSelect))
	{
		TArray<TObjectPtr<ISelectableUnitInterface>> SelectableUnits = GroupSelect();
		PrioritySelect(SelectableUnits);
	} else SingleSelect();
}

void UUnitSelectorComponent::SingleSelect()
{
	//Tries selecting a selectable unit under cursor.
	if (TObjectPtr<ISelectableUnitInterface> Unit = FindSelectableUnitUnderMouseCursor())
	{
		SelectedUnits.Add(Unit);
		Unit->SetPrimarySelected(true);
		Unit->SetSelected(true);
	}
}
void UUnitSelectorComponent::RightClick()
{
	if (SelectedUnits.Num() == 0) return;
	
	//Use the screen-space mouse position and the PlayerController 
	//to get a world position and direction of the mouse from the camera perspective
	FVector WorldPos, WorldDir;
	PlayerController->DeprojectMousePositionToWorld(WorldPos, WorldDir);

	//Use the world position and direction above to generate a linetrace 
	TArray<FHitResult> OutHits;
	FVector TraceStart = WorldPos;
	FVector TraceEnd = WorldPos + WorldDir * TRACE_DISTANCE;
	GetWorld()->LineTraceMultiByChannel(
		OutHits,
		TraceStart,
		TraceEnd,
		TraceChannelTarget
	);

	//Find all overlapped actors that implement ISelectorTargetInterface
	TArray<TObjectPtr<AActor>> Targets;
	for (FHitResult Hit : OutHits)
	{
		TObjectPtr<ISelectorTargetInterface> HitTarget = Cast<ISelectorTargetInterface>(Hit.GetActor());
		if (UKismetSystemLibrary::DoesImplementInterface(Hit.GetActor(), USelectorTargetInterface::StaticClass())
			|| HitTarget)
		{
			Targets.Add(Hit.GetActor());
		}
	}

	//Inform all selected units of their new target information
	if (OutHits.Num() == 0)
	{
		return;
	}
	FVector TargetPosition = OutHits.Last().Location;
	for (TObjectPtr<ISelectableUnitInterface> Unit : SelectedUnits)
	{
		Unit->AssignTarget(TargetPosition, Targets);
	}

}


TArray<TObjectPtr<ISelectableUnitInterface>> UUnitSelectorComponent::GroupSelect()
{
	//Translate screen mouse position to in-world vectors
	FVector WorldPosStart, WorldPosEnd, WorldDirStart, WorldDirEnd;
	PlayerController->DeprojectScreenPositionToWorld(DragStartMousePosition.X, DragStartMousePosition.Y,
		WorldPosStart, WorldDirStart);
	PlayerController->DeprojectScreenPositionToWorld(DragEndMousePosition.X, DragEndMousePosition.Y,
		WorldPosEnd, WorldDirEnd);

	//These two LineTraces find the ground level locations targeted by the mouse during selection.
	FVector SelectionLocationStart, SelectionLocationEnd; //Results stored here
	FVector TraceStart, TraceEnd;
	FHitResult OutHit;

	TraceStart = WorldPosStart;
	TraceEnd = WorldPosStart + WorldDirStart * TRACE_DISTANCE;
	GetWorld()->LineTraceSingleByChannel(
		OutHit,
		TraceStart,
		TraceEnd,
		TraceChannelSelect
	);
	SelectionLocationStart = OutHit.Location;

	TraceStart = WorldPosEnd;
	TraceEnd = WorldPosEnd + WorldDirEnd * TRACE_DISTANCE;
	GetWorld()->LineTraceSingleByChannel(
		OutHit,
		TraceStart,
		TraceEnd,
		TraceChannelSelect
	);
	SelectionLocationEnd = OutHit.Location;

	//Create the center and extents of the box which has SelectionLocationStart & End as edges.
	FVector BoxCenter = (SelectionLocationStart + SelectionLocationEnd) / 2.0f;
	BoxCenter.Z = 0.0f;
	FVector BoxExtents = FVector(
		FMath::Abs(SelectionLocationStart.X - SelectionLocationEnd.X) / 2.0f,
		FMath::Abs(SelectionLocationStart.Y - SelectionLocationEnd.Y) / 2.0f,
		OVERLAP_BOX_HEIGHT);
	
	//Use the box information above to perform a Box Overlap, storing all Actors that are overlapped
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(TraceChannelSelect));
	TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(TraceChannelSelectBuildings));
	TArray<AActor*> IgnoreActors;
	TArray<AActor*> OutActors;
	UKismetSystemLibrary::BoxOverlapActors(
		this,
		BoxCenter,
		BoxExtents,
		TraceObjectTypes,
		AActor::StaticClass(),
		IgnoreActors,
		OutActors
	);
	//Go through the list of actors and see which are selectable, select these. 
	TArray<TObjectPtr<ISelectableUnitInterface>> SelectableActors;
	for (AActor* Unit : OutActors) 
	{
		TObjectPtr<ISelectableUnitInterface> CastUnit = Cast<ISelectableUnitInterface>(Unit);
		if (CastUnit)
		{
			SelectableActors.Add(CastUnit);
		}
	}
	return SelectableActors;
}
void UUnitSelectorComponent::PrioritySelect(TArray<TObjectPtr<ISelectableUnitInterface>> SelectableUnits)
{
	//Find lowest priority in array
	uint8 LowPriority = 255;
	for (TObjectPtr<ISelectableUnitInterface> Unit : SelectableUnits)
	{
		LowPriority = FMath::Min(LowPriority, (uint8)Unit->GetSelectionPriority());
	}
	// If a unit is single-selectable, only select that one
	for (TObjectPtr<ISelectableUnitInterface> Unit : SelectableUnits)
	{
		if (!(Unit->IsGroupSelectable()) && ((uint8)Unit->GetSelectionPriority() == LowPriority))
		{
			SelectedUnits.Add(Unit);
			Unit->SetPrimarySelected(true);
			Unit->SetSelected(true);
			return;
		}
	}

	//Select all objects with lowest priority
	bool bShouldSelectPrimary = true;
	for (TObjectPtr<ISelectableUnitInterface> Unit : SelectableUnits)
	{
		if ((uint8)Unit->GetSelectionPriority() == LowPriority)
		{
			SelectedUnits.Add(Unit);
			if (bShouldSelectPrimary)
			{
				Unit->SetPrimarySelected(true);
				bShouldSelectPrimary = false;
			}
			Unit->SetSelected(true);
		}
	}

}

TObjectPtr<ISelectableUnitInterface> UUnitSelectorComponent::FindSelectableUnitUnderMouseCursor()
{
	//Use the screen-space mouse position and the PlayerController 
	//to get a world position and direction of the mouse from the camera perspective
	FVector WorldPos, WorldDir;
	PlayerController->DeprojectMousePositionToWorld(WorldPos, WorldDir);

	//Use the world position and direction above to generate a linetrace 
	TArray<FHitResult> OutHits;
	FVector TraceStart = WorldPos;
	FVector TraceEnd = WorldPos + WorldDir * TRACE_DISTANCE; //WHAT IS THIS DISTANCE?
	GetWorld()->LineTraceMultiByChannel(
		OutHits,
		TraceStart,
		TraceEnd,
		TraceChannelSelect
	);

	if (OutHits.Num() == 0) { return nullptr; }

	//Find the first actor that has a implements the USelectableUnitInterface 
	//and return that actor.
	for (FHitResult Hit : OutHits)
	{
		TObjectPtr<ISelectableUnitInterface> Unit = Cast<ISelectableUnitInterface>(Hit.GetActor());
		if (Unit) return Unit;			
	}

	//If there are no immediate hits, instead do an Overlap Box from the ground target to find nearby hit.
	FVector BoxCenter = OutHits.Last().Location;
	FVector BoxExtents = FVector::OneVector * SingleSelectionAreaSize;
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	//TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(TraceChannelSelect));
	//TraceObjectTypes.Add(UEngineTypes::ConvertToObjectType(TraceChannelSelectBuildings));
	TArray<AActor*> IgnoreActors;
	TArray<AActor*> OutActors;
	UKismetSystemLibrary::BoxOverlapActors(
		this,
		BoxCenter,
		BoxExtents,
		TraceObjectTypes,
		AActor::StaticClass(),
		IgnoreActors,
		OutActors
	);
	//Return any hit selectable unit
	for (AActor* Unit : OutActors)
	{
		TObjectPtr<ISelectableUnitInterface> CastUnit = Cast<ISelectableUnitInterface>(Unit);
		if (CastUnit)
		{
			return CastUnit;
		}
	}

	//None of the traced actors have a suitable component.
	return nullptr;
}

void UUnitSelectorComponent::ClearSelectedUnits()
{
	for (int i = SelectedUnits.Num() - 1; i >= 0; i--)
	{
		SelectedUnits[i]->SetPrimarySelected(false);
		SelectedUnits[i]->SetSelected(false);
		SelectedUnits.RemoveAt(i);
	}
}

void UUnitSelectorComponent::UnsafeManualSelectList(TArray<AActor*> Units)
{
	ClearSelectedUnits();
	for (int i = 0; i < Units.Num(); i++)
	{
		ISelectableUnitInterface* CastUnit = Cast<ISelectableUnitInterface>(Units[i]);
		if (!CastUnit) return;

		SelectedUnits.Add(CastUnit);
		CastUnit->SetPrimarySelected(!i);
		CastUnit->SetSelected(true);
	}
}

void UUnitSelectorComponent::DeleteSelected()
{
	for (int i = SelectedUnits.Num() - 1; i >= 0; i--)
	{
		TObjectPtr<ISelectableUnitInterface> Unit = SelectedUnits[i];
		AActor* UnitActor = Cast<AActor>(Unit.Get());
		if (UnitActor && ISelectableUnitInterface::Execute_IsDeletable(UnitActor))
		{
			SelectedUnits.RemoveAt(i);
			GetWorld()->DestroyActor(UnitActor);
		}
	}
}