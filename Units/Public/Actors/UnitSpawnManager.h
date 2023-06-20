// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UnitSpawnManager.generated.h"

class UPriceDataAsset;
class AGP3Team16GameModeBase;

UCLASS()
class UNITS_API AUnitSpawnManager : public AActor
{
	GENERATED_BODY()

public:

	//Immediately spawns the unit in PriceData in an empty location near
	//SpawnSource. Does nothing if PriceData contains a non-unit asset.
	UFUNCTION(BlueprintCallable)
	void SpawnUnit(AActor* SpawnSource, UPriceDataAsset* PriceData);
	
protected:

	//Checks locations around the SpawnSource in a spiral pattern
	//Until it finds an empty location. If there are no empty locations
	//it returns the last location in the spiral.
	FVector FindEmptySpawnLocation(AActor* SpawnSource, TSubclassOf<AActor> SpawnAsset);
	float SpiralStepSize = 100.0f;
	int MaxSpiralCount = 16;
	bool IsLocationEmpty(FVector Location, TSubclassOf<AActor> SpawnAsset);
	
	TObjectPtr<AGP3Team16GameModeBase> GameMode;
	AGP3Team16GameModeBase* GetGameMode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Sets default values for this actor's properties
	AUnitSpawnManager();

};
