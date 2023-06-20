#include "Components/PlayerVelocityComponent.h"
#include "Interfaces//CollisionHandlerInterface.h"
#include "Data/PlayerDataAsset.h"

// INITIALISATION

	UPlayerVelocityComponent::UPlayerVelocityComponent(){
		PrimaryComponentTick.bStartWithTickEnabled = true;
		PrimaryComponentTick.bCanEverTick = true;
		bAutoActivate = true;
	}

	void UPlayerVelocityComponent::BeginPlay(){
		Super::BeginPlay();
		Owner = GetOwner();
	}

// COLLISION HANDLING

	void UPlayerVelocityComponent::SetHandler(ICollisionHandlerInterface* Handler){
		CollisionHandler = Handler;
	}

	void UPlayerVelocityComponent::SetObstructed(const bool Obstructed){
		bObstructed = Obstructed;
	}

// MOVEMENT HANDLING

	void UPlayerVelocityComponent::AddVelocity(const FVector& Velocity, const float SpeedMultiplier){
		MoveVelocity += Velocity;

		const float MaxSpeed = PlayerSettings->GetMinMaxMoveSpeed().Y * SpeedMultiplier;
		MoveVelocity = MoveVelocity.GetClampedToSize(-MaxSpeed, MaxSpeed);

		bMoving = MoveVelocity.X != 0 || MoveVelocity.Y != 0;
	}

	void UPlayerVelocityComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction){
		Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
		
		CollisionHandler->SetCollisionState(this, bMoving, MoveVelocity);
		if (!bMoving) return;
		
		if (!bObstructed){
			FVector Location = Owner->GetActorLocation();
			Location += MoveVelocity * DeltaTime;
			
			Owner->SetActorLocation(Location);
		}
		MoveVelocity = FVector::Zero();
		bMoving = false;
	}
