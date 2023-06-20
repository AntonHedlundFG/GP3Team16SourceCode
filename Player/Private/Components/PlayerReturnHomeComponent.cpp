#include "Components/PlayerReturnHomeComponent.h"
#include "Data/PlayerDataAsset.h"
#include "EnhancedInputComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// INITIALISATION
	
	UPlayerReturnHomeComponent::UPlayerReturnHomeComponent(){
		PrimaryComponentTick.bStartWithTickEnabled = false;
		PrimaryComponentTick.bCanEverTick = false;
		bAutoActivate = true;
	}

	void UPlayerReturnHomeComponent::BeginPlay(){
		Super::BeginPlay();
		Owner = GetOwner();

		const auto Location = Owner->GetActorLocation();
		const auto& Home = PlayerSettings->GetHomeLocation();
		LastLocation = FVector(Home.X, Home.Y, Location.Z);
	}

// INPUT HANDLING

	void UPlayerReturnHomeComponent::BindInput(UEnhancedInputComponent* InputComponent){
		if (!HomeInput) return;
		
		InputBinding = &InputComponent->BindAction(
			HomeInput, ETriggerEvent::Started,
			this, &UPlayerReturnHomeComponent::OnMoveInput
		);
	}

	void UPlayerReturnHomeComponent::UnbindInput(UEnhancedInputComponent* InputComponent){
		if (InputBinding)
			InputComponent->RemoveBinding(*InputBinding);
	}

	void UPlayerReturnHomeComponent::OnMoveInput(const FInputActionInstance& Instance){
		if (!IsActive()) return;
		
		const auto& InputValue = Instance.GetValue();

		switch (InputValue.GetValueType()){
			case EInputActionValueType::Boolean:
				OnReturnHome();
				break;

			default: UE_LOG(LogTemp, Warning,
				TEXT("Unsuported ValueType: %s"),
				*UEnum::GetValueAsString(InputValue.GetValueType())
			);
		}
	}

// MOVEMENT HANDLING

	void UPlayerReturnHomeComponent::OnReturnHome(){
		if (!PlayerSettings || !Owner) return;

		const FVector2D Home = PlayerSettings->GetHomeLocation();
		FVector Location = Owner->GetActorLocation();
		
		const float CheckpointTime = PlayerSettings->GetMaxCheckPointTime();
		const float CurrentTime = GetElapsedTime();
		
		if (InsideHome(Location, Home)){
			if (CurrentTime > LastReturnTime + CheckpointTime) return;
			
			Location.X = LastLocation.X;
			Location.Y = LastLocation.Y;
			
		} else {
			LastReturnTime = CurrentTime;
			LastLocation   = Location;
			
			Location.X = Home.X;
			Location.Y = Home.Y;
		}
		Owner->SetActorLocation(Location);
	}

	const bool UPlayerReturnHomeComponent::InsideHome(const FVector& Location, const FVector2D& Home) const {
		const FVector2D Extent = PlayerSettings->GetHomeExtent();

		return Location.X > Home.X - Extent.X &&
			   Location.X < Home.X + Extent.X &&
			   Location.Y > Home.Y - Extent.Y &&
			   Location.Y < Home.Y + Extent.Y;
	}

	const float UPlayerReturnHomeComponent::GetElapsedTime() const {
		return UKismetSystemLibrary::GetGameTimeInSeconds(GetWorld());
	}
