#include "Actors/BuildingActor.h"
#include "Components/BoxComponent.h"

// CONSTRUCTION

	ABuildingActor::ABuildingActor(){
		PrimaryActorTick.bStartWithTickEnabled = false;
		PrimaryActorTick.bCanEverTick = false;

		InitSceneRoot();
		InitBoxCollision();
		InitStaticMesh();
	}

	void ABuildingActor::InitSceneRoot(){
		SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
		SetRootComponent(SceneRoot);
	}

	void ABuildingActor::InitBoxCollision(){
		BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
		
		BoxCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
		BoxCollision->SetGenerateOverlapEvents(false);
		
		BoxCollision->SetupAttachment(SceneRoot);

		/*BoxCollision->AttachToComponent(
			SceneRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale
		);*/
	}

	void ABuildingActor::InitStaticMesh(){
		StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
		StaticMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
		StaticMesh->SetGenerateOverlapEvents(false);
	}

// INITIALISATION

	void ABuildingActor::BeginPlay(){
		Super::BeginPlay();
	}

// BUILDING HANDLING

	void ABuildingActor::SetSelected(bool bSelected){
		OnSelectedStateChange.Broadcast(bSelected);
	}

	void ABuildingActor::SetPrimarySelected(bool bPrimarySelected){
		
	}