#include "Actors/BuildingIndicator.h"
#include "Components/BoxComponent.h"

// CONSTRUCTION

	ABuildingIndicator::ABuildingIndicator(){
		PrimaryActorTick.bStartWithTickEnabled = false;
		PrimaryActorTick.bCanEverTick = false;
	}

// INITIALISATION

	void ABuildingIndicator::BeginPlay(){
		Super::BeginPlay();

		World = GetWorld();
		ActorRotation = GetActorRotation().Quaternion();
		
		InitBoxCollision();
		InitStaticMesh();
	}

	void ABuildingIndicator::InitBoxCollision(){
		BoxCollision = Cast<UBoxComponent>(
			GetComponentByClass(UBoxComponent::StaticClass())
		);
		if (!BoxCollision) UE_LOG(LogTemp, Fatal,
			TEXT("Missing Reference: BoxComponent")
		);
		CollisionBounds = BoxCollision->Bounds;
	}

	void ABuildingIndicator::InitStaticMesh(){
		GetComponents<UStaticMeshComponent>(StaticMeshes);
		
		if (StaticMeshes.Num() == 0) UE_LOG(LogTemp, Fatal,
			TEXT("Missing Reference: StaticMeshComponent")
		);
		for (int i =  StaticMeshes.Num() - 1; i >= 0; --i){
			const auto* StaticMesh = StaticMeshes[i];

			if (!StaticMesh->GetStaticMesh())
				StaticMeshes.RemoveAt(i);
		}
	}

// INDICATOR HANDLING

	const bool ABuildingIndicator::IsObstructed(const FVector& Location){
		const bool Obstructed = World->OverlapAnyTestByChannel(
			Location + CollisionBounds.Origin,
			ActorRotation,
			ObstructionChannel,
			BoxCollision->GetCollisionShape()
		);
		if (bObstructed == Obstructed)
			return bObstructed;
		
		for (int i = 0; i < StaticMeshes.Num(); ++i){
			auto* StaticMesh = StaticMeshes[i];

			for (int j = 0; j < StaticMesh->GetNumMaterials(); ++j){
				StaticMesh->SetMaterial(j, Obstructed ? ObstructedMaterial : NormalMaterial);
			}
		}
		bObstructed = Obstructed;
		return bObstructed;
	}
