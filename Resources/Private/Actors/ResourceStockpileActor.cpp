#include "Actors/ResourceStockpileActor.h"
#include "Components/BoxComponent.h"
#include "Data/ResourceDataAsset.h"
#include "Actors/ResourceActor.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utilities/Utilities.h"

// CONSTRUCTION

	AResourceStockpileActor::AResourceStockpileActor(){
		PrimaryActorTick.bStartWithTickEnabled = false;
		PrimaryActorTick.bCanEverTick = false;

		InitBoxCollision();
		SetRootComponent(BoxTrigger);
	}

	void AResourceStockpileActor::InitBoxCollision(){
		BoxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTrigger"));
		BoxTrigger->SetRelativeLocation(FVector(0, 0, 25));
		BoxTrigger->InitBoxExtent(FVector(500, 500, 50));

		BoxTrigger->SetGenerateOverlapEvents(true);
		BoxTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
		BoxTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // CHANGE PAWN TO WORKER UNIT (EXPOSE TO VARIABLE)
		BoxTrigger->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}

// INITIALISATION

	void AResourceStockpileActor::BeginPlay(){
		Super::BeginPlay();

		if (ValidateReferences()) return;
		//InitTriggerOverlap();
		SpawnResources();
	}

	const bool AResourceStockpileActor::ValidateReferences() const {
		if (!ResourceSettings){
			LOG_SCREEN("Missing ResourceSettings in Stockpile", FColor::Red, 10);
			return false;
		}
		if (!ResourceSettings->GetActor()){
			LOG_SCREEN("Missing Resource in ResourceSettings", FColor::Red, 10);
			return false;
		}
		return true;
	}

	void AResourceStockpileActor::InitTriggerOverlap(){
		BoxTrigger->OnComponentBeginOverlap.AddDynamic(
			this, &AResourceStockpileActor::OnBeginOverlap
		);
	}

// RESOURCE HANDLING

	void AResourceStockpileActor::SpawnResources(){
		const auto& World    = GetWorld();
		const auto& Location = GetActorLocation();
		const auto& Bounds   = BoxTrigger->Bounds;
		const uint8 Count    = UKismetMathLibrary::Sqrt(ResourceCount);
			
		const auto GridStart = FVector2D(
			Location.X - Bounds.BoxExtent.X,
			Location.Y - Bounds.BoxExtent.Y
		);
		const auto GridSize = FVector2D(
			(Bounds.BoxExtent.X * 2) / Count,
			(Bounds.BoxExtent.Y * 2) / Count
		);
		const float Padding = ResourcePadding * 0.5f;
		const float Height  = Location.Z;
		auto GridPosition   = FVector(0, 0, Height);
		
		for (int X = 0; X < Count; ++X){
			for (int Y = 0; Y < Count; ++Y){
				GridPosition.X = GridStart.X + FMath::RandRange(
					(GridSize.X * X) + Padding,
					(GridSize.X * (X + 1)) - Padding
				);
				GridPosition.Y = GridStart.Y + FMath::RandRange(
					(GridSize.Y * Y) + Padding,
					(GridSize.Y * (Y + 1)) - Padding
				);
				auto* ResourceActor = World->SpawnActor<AResourceActor>(
					ResourceSettings->GetActor(), GridPosition, FRotator::ZeroRotator 
				);
				ActiveResources.Add(ResourceActor);
			}
		}
	}

// RESOURCE ASSIGNMENT

	void AResourceStockpileActor::OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
		AssignResource(OtherActor);
	}

	void AResourceStockpileActor::AssignResource(const AActor* Worker){
		const auto& ActorLocation = Worker->GetActorLocation();
		const auto& BoxExtent = BoxTrigger->Bounds.BoxExtent;
		
		const AResourceActor* ClosestResource = nullptr;
		float ClosestDistance = FMath::Square(BoxExtent.X + BoxExtent.Y);
		int ClosestIndex = -1;
		
		for (int i = 0; i < ActiveResources.Num(); ++i){
			const auto* ResourceActor = ActiveResources[i];
			if (!ResourceActor) continue;
			
			const auto& ResourceLocation = ResourceActor->GetActorLocation();
			const float Distance = FVector::DistSquared2D(ResourceLocation, ActorLocation);
			if (Distance > ClosestDistance) continue;

			ClosestDistance = Distance;
			ClosestResource = ResourceActor;
			ClosestIndex = i;
		}

		if (ClosestIndex > INDEX_NONE)
			ActiveResources.RemoveAt(ClosestIndex);

		if (ClosestResource)
			OnAssignedResource(Worker, ClosestResource);
	}

	void AResourceStockpileActor::PickedUpResource(const AResourceActor* ResourceActor){
		LOG_SCREEN("Picked up Resource", FColor::White, 5);
		
		if(ResourceActor){
			//ResourceActor->Hide(true);
			
		} else UE_LOG(LogTemp, Error,
			TEXT("Invalid Resource: %s"), *ResourceActor->GetName()
		);
	}

	void AResourceStockpileActor::AbandonedResource(AResourceActor* ResourceActor){
		LOG_SCREEN("Abandoned Resource", FColor::White, 5);

		if (ResourceActor){
			ActiveResources.Add(ResourceActor);
			
		} else UE_LOG(LogTemp, Error,
			TEXT("Invalid Resource: %s"), *ResourceActor->GetName()
		);
	}
