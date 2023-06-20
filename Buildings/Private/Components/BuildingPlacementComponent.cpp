#include "Components/BuildingPlacementComponent.h"
#include "EnhancedInputComponent.h"
#include "Actors/BuildingSpawnManager.h"
#include "GP3Team16/GP3Team16GameModeBase.h"
#include "Kismet/GameplayStatics.h"

// INITIALISATION

	UBuildingPlacementComponent::UBuildingPlacementComponent(){
		PrimaryComponentTick.bStartWithTickEnabled = false;
		PrimaryComponentTick.bCanEverTick = false;
		bAutoActivate = true;
	}

// INPUT HANDLING

	void UBuildingPlacementComponent::BindInput(UEnhancedInputComponent* InputComponent){
		PlaceInputBindingCompleted = &InputComponent->BindAction(
			PlaceBuildingInput, ETriggerEvent::Started,
			this, &UBuildingPlacementComponent::OnPlaceBuildingInput
		);
		CancelInputBindingCompleted = &InputComponent->BindAction(
			CancelBuildingInput, ETriggerEvent::Completed,
			this, &UBuildingPlacementComponent::OnCancelBuildingInput
		);
	}

	void UBuildingPlacementComponent::UnbindInput(UEnhancedInputComponent* InputComponent){
		if (PlaceInputBindingCompleted)
			InputComponent->RemoveBinding(*PlaceInputBindingCompleted);
		
		if (CancelInputBindingCompleted)
			InputComponent->RemoveBinding(*CancelInputBindingCompleted);
	}

	void UBuildingPlacementComponent::OnPlaceBuildingInput(const FInputActionInstance& Instance){
		const auto& InputValue = Instance.GetValue();

		switch (InputValue.GetValueType()){
			case EInputActionValueType::Boolean:
				GetBuildingManager()->PlaceBuilding();
				break;

			default: UE_LOG(LogTemp, Warning,
				TEXT("Unsuported ValueType: %s"),
				*UEnum::GetValueAsString(InputValue.GetValueType())
			);
		}
	}

	void UBuildingPlacementComponent::OnCancelBuildingInput(const FInputActionInstance& Instance){
		const auto& InputValue = Instance.GetValue();

		switch (InputValue.GetValueType()){
			case EInputActionValueType::Boolean:
				GetBuildingManager()->CancelPlacingBuilding();
				break;

			default: UE_LOG(LogTemp, Warning,
				TEXT("Unsuported ValueType: %s"),
				*UEnum::GetValueAsString(InputValue.GetValueType())
			);
		}
	}

	ABuildingSpawnManager* UBuildingPlacementComponent::GetBuildingManager()
	{
		if (!BuildingManager)
		{
			const auto* GameMode = Cast<AGP3Team16GameModeBase>(
				UGameplayStatics::GetGameMode(GetWorld())
				);
			if (!GameMode) UE_LOG(LogTemp, Fatal,
				TEXT("Failed Cast: GP3Team16GameModeBase")
			);
			BuildingManager = GameMode->GetBuildingSpawnManager();
			if (!BuildingManager) UE_LOG(LogTemp, Fatal,
				TEXT("Missing Reference: BuildingSpawnManager")
			);
		}
		return BuildingManager;
	}
