#include "Components/PlayerCollisionComponent.h"
#include "Interfaces/CollidableInterface.h"

// INITIALISATION

	UPlayerCollisionComponent::UPlayerCollisionComponent(){
		PrimaryComponentTick.bStartWithTickEnabled = false;
		PrimaryComponentTick.bCanEverTick = true;
		bAutoActivate = true;
	}

	void UPlayerCollisionComponent::BeginPlay(){
		Super::BeginPlay();
		
		Owner = GetOwner();
		World = GetWorld();
		
		InitCollidables();
		SetComponentTickEnabled(false);
	}

	void UPlayerCollisionComponent::InitCollidables(){
		const auto Components = Owner->GetComponentsByInterface(
			UCollidableInterface::StaticClass()
		);
		for (int i = 0; i < Components.Num(); ++i){
			auto* Collidable = Cast<ICollidableInterface>(Components[i]);

			if (!Collidable) continue;
			Collidable->SetHandler(this);
			
			Collidables.Add(Collidable);
			CollisionStates.Add(false);
			CollisionDirections.Add(FVector::Zero());
		}
	}

// COLLISION HANDLING

	void UPlayerCollisionComponent::SetCollisionState(ICollidableInterface* Collidable, const bool CheckCollision, FVector Direction){
		const uint8 Index = Collidables.Find(Collidable);
		
		if (CollisionStates.Num() > Index)
			CollisionStates[Index] = CheckCollision;

		if (CollisionDirections.Num() > Index){
			if (CheckCollision && !Direction.IsNormalized())
				Direction.Normalize();
			
			CollisionDirections[Index] = CheckCollision ?
				Direction : FVector::Zero();
		}
		CheckDirection = GetCheckDirection();
		
		const bool TickState = GetTickState();
		if (bTicking == TickState) return;
		
		bTicking = TickState;
		SetComponentTickEnabled(bTicking);
	}

	void UPlayerCollisionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction){
		Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

		const FVector StartLocation = Owner->GetActorLocation();
		const FVector EndLocation = StartLocation + CheckDirection * 1000;
		FHitResult OutHit;

		const bool Obstructed = World->LineTraceSingleByChannel(
			OutHit, StartLocation, EndLocation, CollisionChannel
		);

		for (int i = 0; i < Collidables.Num(); ++i){
			auto Collidable = Collidables[i];
			
			if (!Collidable) continue;
			Collidable->SetObstructed(Obstructed);
		}
	}


	bool UPlayerCollisionComponent::GetTickState(){
		uint8 NewState = 0;
		
		for (const uint8 State : CollisionStates)
			NewState += State;

		return NewState > 0;
	}
	
	FVector UPlayerCollisionComponent::GetCheckDirection(){
		const uint8 Count = CollisionDirections.Num();
		FVector Direction = FVector::Zero();
		
		for (int i = 0; i < Count; ++i){
			const auto& ColDir = CollisionDirections[i];
			Direction.X += ColDir.X;
			Direction.Y += ColDir.Y;
		}
		Direction.X /= Count;
		Direction.Y /= Count;
		Direction.Normalize();

		return Direction;
	}