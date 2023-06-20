#include "Actors/BuildingSpawnManager.h"
#include "GP3Team16/GP3Team16GameModeBase.h"
#include "Data/PriceDataAsset.h"
#include "Kismet/GameplayStatics.h"

// INITIALISATION

	ABuildingSpawnManager::ABuildingSpawnManager(){
		PrimaryActorTick.bStartWithTickEnabled = false;
		PrimaryActorTick.bCanEverTick = true;
	}

	void ABuildingSpawnManager::BeginPlay(){
		Super::BeginPlay();
		World = GetWorld();
		
		InitGameMode();
		InitPurchaseEvent();
		InitPlayerController();
	}

	void ABuildingSpawnManager::InitGameMode(){
		GameMode = Cast<AGP3Team16GameModeBase>(
			UGameplayStatics::GetGameMode(GetWorld())
		);
		if (!GameMode) UE_LOG(LogTemp, Fatal,
			TEXT("Missing Reference: GameMode")
		);
	}

	void ABuildingSpawnManager::InitPurchaseEvent(){
		if (!GameMode) return;

		GameMode->OnPurchaseBuilding.BindUObject(
			this, &ABuildingSpawnManager::PurchaseBuilding
		);
	}

	void ABuildingSpawnManager::InitPlayerController(){
		if (!World) return;
		
		PlayerController = UGameplayStatics::GetPlayerController(World, 0);

		if (!PlayerController) UE_LOG(LogTemp, Fatal,
			TEXT("Missing Reference: PlayerController")
		);
	}

	// Assigned from BuildPlane Blueprint on BeginPlay
	void ABuildingSpawnManager::AssignBuildPlane(UStaticMeshComponent* BuildPlane){
		BuildPlaneVisuals = BuildPlane;

		if (!BuildPlane) UE_LOG(LogTemp, Error,
			TEXT("Missing Reference: BuildPlane")
		);
	}

// BUILDING HANDLING

	void ABuildingSpawnManager::PurchaseBuilding(AActor* Source, UPriceDataAsset* Purchase){
		if (!Purchase) return;

		if (bPlacingBuilding){
			CancelPlacingBuilding();
		}
		Purchased = Purchase;
		GameMode->ClearSelectedUnits();
		
		StartPlacingBuilding();
	}

	void ABuildingSpawnManager::StartPlacingBuilding(){
		BuildingIndicator = World->SpawnActor<ABuildingIndicator>(
			Purchased->GetIndicatorAsset()
		);
		if (!BuildingIndicator) return;

		if (BuildPlaneVisuals){
			BuildPlaneVisuals->SetVisibility(true);
		}
		bPlacingBuilding = true;
		SetActorTickEnabled(true);
	}

	void ABuildingSpawnManager::CancelPlacingBuilding(){
		if (!bPlacingBuilding || !Purchased) return;
		
		Purchased->Refund(GameMode);
		CompletePlacingBuilding();
	}

	void ABuildingSpawnManager::PlaceBuilding(){
		if (bObstructed || !bPlacingBuilding || !Purchased) return;
		
		const FVector  Location = BuildingIndicator->GetActorLocation();
		const FRotator Rotation = BuildingIndicator->GetActorRotation();
		World->SpawnActor(Purchased->GetObjectAsset(), &Location, &Rotation);
		
		CompletePlacingBuilding();
	}

	void ABuildingSpawnManager::CompletePlacingBuilding(){
		if (BuildingIndicator){
			World->DestroyActor(BuildingIndicator);
		}
		if (BuildPlaneVisuals){
			BuildPlaneVisuals->SetVisibility(false);
		}
		Purchased = nullptr;
		bPlacingBuilding = false;
		SetActorTickEnabled(false);
	}


	void ABuildingSpawnManager::Tick(float DeltaSeconds){
		Super::Tick(DeltaSeconds);

		if (!bPlacingBuilding) return;
		const FVector WorldLocation = GetWorldPosition();
		
		BuildingIndicator->SetActorLocation(WorldLocation);
		bObstructed = BuildingIndicator->IsObstructed(WorldLocation);
	}

	FVector ABuildingSpawnManager::GetWorldPosition(){
		FHitResult Result;
		
		PlayerController->DeprojectMousePositionToWorld(
			MouseWorldPosition, MouseWorldDirection
		);
		FVector EndPosition = MouseWorldPosition;
		EndPosition += MouseWorldDirection * 100000;
		
		const bool Hit = World->LineTraceSingleByChannel(
			Result, MouseWorldPosition, EndPosition, GroundChannel
		);
		if (Hit) LastValidPosition = Result.Location;
		return LastValidPosition;
	}