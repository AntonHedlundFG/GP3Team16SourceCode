#include "Components/PlayerCursorComponent.h"
#include "Actors/DirectControlledUnitActor.h"
//#include "Actors/BuildingActor.h"
#include "Kismet/GameplayStatics.h"

// INITIALISATION

	UPlayerCursorComponent::UPlayerCursorComponent(){
		PrimaryComponentTick.bStartWithTickEnabled = true;
		PrimaryComponentTick.bCanEverTick = true;
		bAutoActivate = true;
	}

	void UPlayerCursorComponent::BeginPlay(){
		Super::BeginPlay();
		World = GetWorld();

		InitPlayerController();
	}

	void UPlayerCursorComponent::InitPlayerController(){
		if (!World) return;
		
		PlayerController = UGameplayStatics::GetPlayerController(World, 0);

		if (!PlayerController) UE_LOG(LogTemp, Fatal,
			TEXT("Missing Reference: PlayerController")
		);
	}

// CURSOR HANDLING

	void UPlayerCursorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction){
		Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

		FVector Start, End;
		GetTracePositions(Start, End);
		
		FHitResult HitResult;
		HitResult = GetMouseHitResult(Start, End, SelectionChannel);
		//if (SelectedActor(HitResult)) OnMouseOver(SelectedUnit);

		//HitResult = GetMouseHitResult(Start, End, BuildingChannel);
		//if (SelectedActor(HitResult)) OnMouseOverBuilding(SelectedBuilding);
	}

	const void UPlayerCursorComponent::GetTracePositions(FVector& Start, FVector& End) const {
		FVector MousePosition, MouseDirection;
		
		PlayerController->DeprojectMousePositionToWorld(
			MousePosition, MouseDirection
		);
		Start = MousePosition;
		End   = Start + (MouseDirection * 100000);
	}

	const FHitResult UPlayerCursorComponent::GetMouseHitResult(const FVector& Start, const FVector& End, const ECollisionChannel CollisionChannel) const {
		FHitResult HitResult;
		
		World->LineTraceSingleByChannel(
			HitResult, Start, End, CollisionChannel
		);
		return HitResult;
	}

	const bool UPlayerCursorComponent::SelectedActor(const FHitResult& HitResult){
		if (!HitResult.bBlockingHit) return false;

		const auto* Component = HitResult.Component.Get();
		if (!Component) return false;

		auto* Actor = Component->GetOwner();
		SelectedUnit = Cast<ADirectControlledUnitActor>(Actor);
		if (SelectedUnit) return true;

		//SelectedBuilding = Cast<ABuildingActor>(Actor);
		//if (SelectedBuilding) return true;

		return false;
	}
