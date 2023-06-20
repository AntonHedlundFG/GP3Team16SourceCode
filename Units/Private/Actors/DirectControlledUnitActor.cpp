#include "Actors/DirectControlledUnitActor.h"
#include "Interfaces/SelectorTargetInterface.h"
#include "Enums/SelectorTargetType.h"

// Sets default values
ADirectControlledUnitActor::ADirectControlledUnitActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADirectControlledUnitActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADirectControlledUnitActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

EUnitSelectionPriority ADirectControlledUnitActor::GetSelectionPriority()
{
	return SelectionPriority;
}

bool ADirectControlledUnitActor::IsGroupSelectable()
{
	return bIsGroupSelectable;
}

void ADirectControlledUnitActor::SetSelected(bool bSelected)
{
	OnSelectedStateChange.Broadcast(bSelected);
}

void ADirectControlledUnitActor::AssignTarget(FVector TargetWorldPosition, TArray<TObjectPtr<AActor>> Targets)
{
	for (TObjectPtr<AActor> Target : Targets)
	{
		ESelectorTargetType Type = Cast<ISelectorTargetInterface>(Target)->Execute_GetSelectorType(Target);
		if (SelectorTargets & (uint8)Type)
		{
			OnNewActorTarget.Broadcast(Target);
			return;
		}
	}

	OnNewWorldPositionTarget.Broadcast(TargetWorldPosition);
}

void ADirectControlledUnitActor::SetPrimarySelected(bool bPrimarySelected)
{
	bIsPrimarySelected = bPrimarySelected;
}