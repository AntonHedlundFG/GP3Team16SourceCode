#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingSpawnManager.generated.h"

class ABuildingIndicator;
class UPriceDataAsset;
class APlayerController;
class AGP3Team16GameModeBase;

UCLASS()
class BUILDINGS_API ABuildingSpawnManager : public AActor
{
	GENERATED_BODY()
	
public:
	
	ABuildingSpawnManager();

	UFUNCTION(BlueprintCallable)
	void AssignBuildPlane(UStaticMeshComponent* BuildPlane);
	
	void PurchaseBuilding(AActor* Source, UPriceDataAsset* Purchase);
	void CancelPlacingBuilding();
	void PlaceBuilding();
	
protected:

	UPROPERTY(Category="Buildings", EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<ECollisionChannel> GroundChannel = ECC_GameTraceChannel8;

	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
private:

	bool bPlacingBuilding = false;
	bool bObstructed = false;
	
	FVector MouseWorldPosition = FVector::Zero();
	FVector MouseWorldDirection = FVector::Zero();
	FVector LastValidPosition = FVector::Zero();
		
	TObjectPtr<UPriceDataAsset> Purchased;
	TObjectPtr<AGP3Team16GameModeBase> GameMode;
	
	UPROPERTY() UWorld* World = nullptr;
	UPROPERTY() ABuildingIndicator* BuildingIndicator = nullptr;
	UPROPERTY() UStaticMeshComponent* BuildPlaneVisuals = nullptr;
	UPROPERTY() APlayerController* PlayerController = nullptr;
	UPROPERTY() ACameraActor* Camera = nullptr;

	
	void InitGameMode();
	void InitPurchaseEvent();
	void InitPlayerController();
	
	void StartPlacingBuilding();
	void CompletePlacingBuilding();
	
	FVector GetWorldPosition();
	
};
