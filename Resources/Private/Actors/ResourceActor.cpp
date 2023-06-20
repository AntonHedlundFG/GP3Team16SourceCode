#include "Actors/ResourceActor.h"

// CONSTRUCTION

	AResourceActor::AResourceActor(){
		PrimaryActorTick.bStartWithTickEnabled = false;
		PrimaryActorTick.bCanEverTick = false;

		InitStaticMesh();
		SetRootComponent(StaticMesh);
	}

	void AResourceActor::InitStaticMesh(){
		StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	}

// INITIALISATION

	void AResourceActor::BeginPlay(){
		Super::BeginPlay();
	}

// RESOURCE HANDLING

	void AResourceActor::Hide(const bool Hidden) const {
		if (!StaticMesh) return;
		
		StaticMesh->SetVisibility(Hidden);
	}
