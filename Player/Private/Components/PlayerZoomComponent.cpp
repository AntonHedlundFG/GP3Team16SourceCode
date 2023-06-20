#include "Components/PlayerZoomComponent.h"
#include "Data/PlayerDataAsset.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"

// INITIALISATION

	UPlayerZoomComponent::UPlayerZoomComponent(){
		PrimaryComponentTick.bStartWithTickEnabled = false;
		PrimaryComponentTick.bCanEverTick = false;
		bAutoActivate = true;
	}

	void UPlayerZoomComponent::BeginPlay(){
		Super::BeginPlay();
		InitSpringArmComponent();
		InitZoomDistance();
	}

	void UPlayerZoomComponent::InitSpringArmComponent(){
		const auto* Owner = GetOwner();

		SpringArm = Cast<USpringArmComponent>(
			Owner->GetComponentByClass(
				USpringArmComponent::StaticClass()
			)
		);

		if (!SpringArm) UE_LOG(LogTemp, Fatal,
			TEXT("Missing Component: SpringArmComponent")
		);
	}

	void UPlayerZoomComponent::InitZoomDistance() const {
		const auto& MinMaxZoom = PlayerSettings->GetMinMaxZoomDistance();
		const float StartZoom = PlayerSettings->GetStartingZoomDistance();
		
		SpringArm->TargetArmLength = FMath::Clamp(
			StartZoom, MinMaxZoom.X, MinMaxZoom.Y
		);
	}

// INPUT HANDLING

	void UPlayerZoomComponent::BindInput(UEnhancedInputComponent* InputComponent){
		InputBindingTriggered = &InputComponent->BindAction(
			ZoomInput, ETriggerEvent::Triggered,
			this, &UPlayerZoomComponent::OnZoomInput
		);
	}

	void UPlayerZoomComponent::UnbindInput(UEnhancedInputComponent* InputComponent){
		if (InputBindingTriggered)
			InputComponent->RemoveBinding(*InputBindingTriggered);
	}

	void UPlayerZoomComponent::OnZoomInput(const FInputActionInstance& Instance){
		if (!IsActive()) return;
		const auto& InputValue = Instance.GetValue();

		switch (InputValue.GetValueType()){
			case EInputActionValueType::Axis1D:
				OnZoom(InputValue.Get<float>());
				break;

			default: UE_LOG(LogTemp, Warning,
				TEXT("Unsuported ValueType: %s"),
				*UEnum::GetValueAsString(InputValue.GetValueType())
			);
		}
	}

// ZOOM HANDLING

	void UPlayerZoomComponent::OnZoom(const float Direction) const {
		const auto& MinMaxZoom = PlayerSettings->GetMinMaxZoomDistance();
		const float Distance = Direction * PlayerSettings->GetZoomSpeed();
		
		SpringArm->TargetArmLength = FMath::Clamp(
			SpringArm->TargetArmLength - Distance,
			MinMaxZoom.X, MinMaxZoom.Y
		);
	}
