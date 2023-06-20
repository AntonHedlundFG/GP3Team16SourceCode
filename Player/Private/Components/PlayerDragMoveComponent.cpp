#include "Components/PlayerDragMoveComponent.h"
#include "Components/PlayerVelocityComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Data/PlayerDataAsset.h"
#include "EnhancedInputComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

// INITIALISATION

	UPlayerDragMoveComponent::UPlayerDragMoveComponent(){
		PrimaryComponentTick.bStartWithTickEnabled = false;
		PrimaryComponentTick.bCanEverTick = true;
		bAutoActivate = true;
	}
	
	void UPlayerDragMoveComponent::BeginPlay(){
		Super::BeginPlay();
		
		Owner = GetOwner();
		PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		Rotation = Owner->GetActorRotation();

		InitSpringArmComponent();
		InitVelocityComponent();
		InitViewPort();
		
		// Added because of a weird bug where tick gets enabled on start 
		SetComponentTickEnabled(false);
	}

	void UPlayerDragMoveComponent::InitSpringArmComponent(){
		SpringArmComponent = Cast<USpringArmComponent>(
			Owner->GetComponentByClass(USpringArmComponent::StaticClass())
		);
		if (!SpringArmComponent) UE_LOG(LogTemp, Fatal,
			TEXT("Missing Reference: SpringArmComponent")
		);
	}

	void UPlayerDragMoveComponent::InitVelocityComponent(){
		VelocityComponent = Cast<UPlayerVelocityComponent>(
			Owner->GetComponentByClass(UPlayerVelocityComponent::StaticClass())
		);
		if (!VelocityComponent) UE_LOG(LogTemp, Fatal,
			TEXT("Missing Reference: PlayerVelocityComponent")
		);
	}

	void UPlayerDragMoveComponent::InitViewPort(){
		ViewPort = PlayerController->GetLocalPlayer()->ViewportClient->Viewport;
	}

// INPUT HANDLING

	void UPlayerDragMoveComponent::BindInput(UEnhancedInputComponent* InputComponent){
		InputBindingTriggered = &InputComponent->BindAction(
			DragInput, ETriggerEvent::Started,
			this, &UPlayerDragMoveComponent::OnDragInput
		);
		
		InputBindingCompleted = &InputComponent->BindAction(
			DragInput, ETriggerEvent::Completed,
			this, &UPlayerDragMoveComponent::OnDragInput
		);
	}

	void UPlayerDragMoveComponent::UnbindInput(UEnhancedInputComponent* InputComponent){
		if (InputBindingTriggered)
			InputComponent->RemoveBinding(*InputBindingTriggered);
		
		if (InputBindingCompleted)
			InputComponent->RemoveBinding(*InputBindingCompleted);
	}

	void UPlayerDragMoveComponent::OnDragInput(const FInputActionInstance& Instance){
		if (!IsActive())
		{
			OnDrag(false);
			return;
		}
		const auto& InputValue = Instance.GetValue();
		
		switch (InputValue.GetValueType()){
			case EInputActionValueType::Boolean:
				OnDrag(InputValue.Get<bool>());
				break;

			default: UE_LOG(LogTemp, Warning,
				TEXT("Unsuported ValueType: %s"),
				*UEnum::GetValueAsString(InputValue.GetValueType())
			);
		}
	}

// MOVEMENT HANDLING

	void UPlayerDragMoveComponent::OnDrag(const bool Dragging){
		if (bMoving == Dragging) return;
		bMoving = Dragging;

		if (bMoving){
			PlayerController->GetMousePosition(
				StartingMousePos.X,
				StartingMousePos.Y
			);
			StartLocation = Owner->GetActorLocation();
			ViewPort->GetMousePos(StartingMouseViewPortPos, true);
			
		} else ViewPort->SetMouse(StartingMouseViewPortPos.X, StartingMouseViewPortPos.Y);
		
		PlayerController->bShowMouseCursor = !bMoving;
		SetComponentTickEnabled(bMoving);
	}

	void UPlayerDragMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction){
		Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
		if (!bMoving) return;

		PlayerController->GetMousePosition(
			CurrentMousePos.X, CurrentMousePos.Y
		);
		const float Multiplier = PlayerSettings->GetDragMoveSpeedMultiplier();
		MoveDirection = GetMoveDirection();
		
		VelocityComponent->AddVelocity(
			MoveDirection * GetMoveSpeed() * Multiplier, Multiplier
		);
	}

	FVector UPlayerDragMoveComponent::GetMoveDirection() const {
		const FVector2D Distance = CurrentMousePos - StartingMousePos;
		
		FVector Direction = FVector(-Distance.Y, Distance.X, 0);
				Direction = Rotation.RotateVector(Direction);

		return Direction;
	}

	const float UPlayerDragMoveComponent::GetMoveSpeed() const {
		const auto& MinMaxSpeed = PlayerSettings->GetMinMaxMoveSpeed();
		const auto& MinMaxZoom  = PlayerSettings->GetMinMaxZoomDistance();

		const float Strength = UKismetMathLibrary::NormalizeToRange(
			SpringArmComponent->TargetArmLength, MinMaxZoom.X, MinMaxZoom.Y
		);
		return FMath::Lerp(MinMaxSpeed.X, MinMaxSpeed.Y, Strength);
	}