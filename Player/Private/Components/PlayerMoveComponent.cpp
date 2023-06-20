#include "Components/PlayerMoveComponent.h"
#include "Data/PlayerDataAsset.h"
#include "EnhancedInputComponent.h"
#include "Components/PlayerVelocityComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

// INITIALISATION

	UPlayerMoveComponent::UPlayerMoveComponent(){
		PrimaryComponentTick.bStartWithTickEnabled = false;
		PrimaryComponentTick.bCanEverTick = true;
		bAutoActivate = true;
	}

	void UPlayerMoveComponent::BeginPlay(){
		Super::BeginPlay();
		
		Owner = GetOwner();
		Rotation = Owner->GetActorRotation();

		InitSpringArmComponent();
		InitVelocityComponent();
	}

	void UPlayerMoveComponent::InitSpringArmComponent(){
		SpringArmComponent = Cast<USpringArmComponent>(
			Owner->GetComponentByClass(USpringArmComponent::StaticClass())
		);
		if (!SpringArmComponent) UE_LOG(LogTemp, Fatal,
			TEXT("Missing Reference: SpringArmComponent")
		);
	}

	void UPlayerMoveComponent::InitVelocityComponent(){
		VelocityComponent = Cast<UPlayerVelocityComponent>(
			Owner->GetComponentByClass(UPlayerVelocityComponent::StaticClass())
		);
		if (!VelocityComponent) UE_LOG(LogTemp, Fatal,
			TEXT("Missing Reference: PlayerVelocityComponent")
		);
	}

// INPUT HANDLING

	void UPlayerMoveComponent::BindInput(UEnhancedInputComponent* InputComponent){
		InputBindingTriggered = &InputComponent->BindAction(
			MoveInput, ETriggerEvent::Triggered,
			this, &UPlayerMoveComponent::OnMoveInput
		);
		
		InputBindingCompleted = &InputComponent->BindAction(
			MoveInput, ETriggerEvent::Completed,
			this, &UPlayerMoveComponent::OnMoveInput
		);
	}

	void UPlayerMoveComponent::UnbindInput(UEnhancedInputComponent* InputComponent){
		if (InputBindingTriggered)
			InputComponent->RemoveBinding(*InputBindingTriggered);
		
		if (InputBindingCompleted)
			InputComponent->RemoveBinding(*InputBindingCompleted);
	}

	void UPlayerMoveComponent::OnMoveInput(const FInputActionInstance& Instance){
		if (!IsActive()){
			OnMove(FVector2D::Zero());
			return;
		}
		const auto& InputValue = Instance.GetValue();

		switch (InputValue.GetValueType()){
			case EInputActionValueType::Axis2D:
				OnMove(InputValue.Get<FVector2D>());
				break;

			default: UE_LOG(LogTemp, Warning,
				TEXT("Unsuported ValueType: %s"),
				*UEnum::GetValueAsString(InputValue.GetValueType())
			);
		}
	}

// MOVEMENT HANDLING

	void UPlayerMoveComponent::OnMove(const FVector2D& Direction){
		if (InputDirection == Direction) return;
		InputDirection = Direction;
		
		MoveDirection.X = Direction.X;
		MoveDirection.Y = Direction.Y;
		MoveDirection = Rotation.RotateVector(MoveDirection);

		bMoving = Direction.X != 0 || Direction.Y != 0;

		if (bMoving == bTicking) return;
		bTicking = bMoving;
		
		SetComponentTickEnabled(bTicking);
	}

	void UPlayerMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction){
		Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
		const float Multiplier = PlayerSettings->GetKeysMoveSpeedMultiplier();
		
		VelocityComponent->AddVelocity(
			MoveDirection * GetMoveSpeed() * Multiplier, Multiplier
		);
	}

	const float UPlayerMoveComponent::GetMoveSpeed() const {
		const auto& MinMaxSpeed = PlayerSettings->GetMinMaxMoveSpeed();
		const auto& MinMaxZoom  = PlayerSettings->GetMinMaxZoomDistance();

		const float Strength = UKismetMathLibrary::NormalizeToRange(
			SpringArmComponent->TargetArmLength, MinMaxZoom.X, MinMaxZoom.Y
		);
		return FMath::Lerp(MinMaxSpeed.X, MinMaxSpeed.Y, Strength);
	}
	