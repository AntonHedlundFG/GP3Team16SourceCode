// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/UnitSpawnManager.h"
#include "Data/PriceDataAsset.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "Actors/DirectControlledUnitActor.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GP3Team16/GP3Team16GameModeBase.h"

// Sets default values
AUnitSpawnManager::AUnitSpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AUnitSpawnManager::BeginPlay()
{
	Super::BeginPlay();
	if(GetGameMode())
		GetGameMode()->OnPurchaseUnit.BindUObject(this, &AUnitSpawnManager::SpawnUnit);
}
AGP3Team16GameModeBase* AUnitSpawnManager::GetGameMode()
{
	if (!GameMode)
		GameMode = Cast<AGP3Team16GameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	return GameMode;
}

void AUnitSpawnManager::SpawnUnit(AActor* SpawnSource, UPriceDataAsset* PriceData)
{
	//Just making sure we're actually spawning a unit. This should never be a problem
	check(PriceData->GetType() == EPurchasableType::PT_Unit);

	//Set spawn settings and spawn the requested Actor.
	TSubclassOf<AActor> SpawnType = PriceData->GetObjectAsset();
	FVector Location = FindEmptySpawnLocation(SpawnSource, SpawnType) + FVector(0.0f, 0.0f, 100.0f);
	FRotator Rotation;
	AActor* SpawnedActor = GetWorld()->SpawnActor(
		SpawnType,
		&Location,
		&Rotation
	);
}

FVector AUnitSpawnManager::FindEmptySpawnLocation(AActor* SpawnSource, TSubclassOf<AActor> SpawnAsset)
{
	//The logic here is a bit hard to explain. We take steps in an outwards growing spiral.
	//First, 1 step up, 1 step right, then 2 steps down, 2 steps left. 3 up, 3 right
	//4 down, 4 left, and so on. Each step along the way we check the location
	//to see if it's empty.
	//This way we find an empty location as close to the building as possible.
	int Sign = 1;
	FVector SpawnLocation = SpawnSource->GetActorLocation() + FVector(0, 0, 100);
	for (int i = 1; i <= MaxSpiralCount; i++)
	{
		for (int j = 0; j < i; j++) 
		{
			SpawnLocation.X += SpiralStepSize * Sign; //Up or down depending on Sign
			if (IsLocationEmpty(SpawnLocation, SpawnAsset))
				return SpawnLocation;
		}
		for (int j = 0; j < i; j++)
		{
			SpawnLocation.Y += SpiralStepSize * Sign; //Left or right depending on Sign
			if (IsLocationEmpty(SpawnLocation, SpawnAsset))
				return SpawnLocation;
		}
		Sign *= -1; //Invert sign, switching from up&left to down&right, or vice versa
	}
	return SpawnLocation;
}

bool AUnitSpawnManager::IsLocationEmpty(FVector Location, TSubclassOf<AActor> SpawnAsset)
{
	FNavLocation OutLocation;
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(this);
	NavSystem->ProjectPointToNavigation(Location, OutLocation);
	FVector NavLocation = OutLocation.Location;
	Location.Z = 0;
	NavLocation.Z = 0;
	
	if (FVector::DistSquared(NavLocation, Location) > 5.0f) return false;

	//A simple box overlap to find any units blocking the location.
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;
	TArray<AActor*> IgnoreActors;
	TArray<AActor*> OutActors;
	UKismetSystemLibrary::BoxOverlapActors(
		this,
		Location,
		FVector(SpiralStepSize, SpiralStepSize, SpiralStepSize * 1.5f) * 2.0f,
		TraceObjectTypes,
		ADirectControlledUnitActor::StaticClass(),
		IgnoreActors,
		OutActors
	);
	return OutActors.Num() == 0;
}