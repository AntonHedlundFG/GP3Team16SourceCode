#include "Components/PlayerMouseMoveComponent.h"
#include "Components/PlayerVelocityComponent.h"
#include "Data/PlayerDataAsset.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

// INITIALISATION

	UPlayerMouseMoveComponent::UPlayerMouseMoveComponent(){
		PrimaryComponentTick.bStartWithTickEnabled = true;;
		PrimaryComponentTick.bCanEverTick = true;
		bAutoActivate = true;
	}

	void UPlayerMouseMoveComponent::BeginPlay(){
		Super::BeginPlay();

		Owner = GetOwner();
		Rotation = Owner->GetActorRotation();

		PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		Viewport = GEngine->GameViewport;
		
		InitSpringArmComponent();
		InitVelocityComponent();
	}

	void UPlayerMouseMoveComponent::InitSpringArmComponent(){
		SpringArmComponent = Cast<USpringArmComponent>(
			Owner->GetComponentByClass(USpringArmComponent::StaticClass())
		);
		if (!SpringArmComponent) UE_LOG(LogTemp, Fatal,
			TEXT("Missing Reference: SpringArmComponent")
		);
	}

	void UPlayerMouseMoveComponent::InitVelocityComponent(){
		VelocityComponent = Cast<UPlayerVelocityComponent>(
			Owner->GetComponentByClass(UPlayerVelocityComponent::StaticClass())
		);
		if (!VelocityComponent) UE_LOG(LogTemp, Fatal,
			TEXT("Missing Reference: PlayerVelocityComponent")
		);
	}

// MOVEMENT HANDLING

	void UPlayerMouseMoveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction){
		Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

		bMoving = ValidMoveDirection();
		if (!bMoving) return;

		const float Multiplier = PlayerSettings->GetScreenMoveSpeedMultiplier();
		MoveDirection = GetFixedMoveDirection();
		
		VelocityComponent->AddVelocity(
			MoveDirection * GetMoveSpeed() * Multiplier, Multiplier
		);
	}

	const bool UPlayerMouseMoveComponent::ValidMoveDirection(){
		const auto& ScreenSize  = Viewport->Viewport->GetSizeXY();
		const float BoarderSize = PlayerSettings->GetScreenBoarderPixelSize();
		
		PlayerController->GetMousePosition(MousePosition.X, MousePosition.Y);

		MoveDirection.X = MousePosition.X > ScreenSize.X - BoarderSize ? 1 :
						  MousePosition.X < BoarderSize ? -1 : 0;

		MoveDirection.Y = MousePosition.Y > ScreenSize.Y - BoarderSize ? 1 :
						  MousePosition.Y < BoarderSize ? -1 : 0;
		
		return MoveDirection.X != 0 || MoveDirection.Y != 0;
	}

	const float UPlayerMouseMoveComponent::GetMoveSpeed() const {
		const auto& MinMaxSpeed = PlayerSettings->GetMinMaxMoveSpeed();
		const auto& MinMaxZoom  = PlayerSettings->GetMinMaxZoomDistance();

		const float Strength = UKismetMathLibrary::NormalizeToRange(
			SpringArmComponent->TargetArmLength, MinMaxZoom.X, MinMaxZoom.Y
		);
		return FMath::Lerp(MinMaxSpeed.X, MinMaxSpeed.Y, Strength);
	}

	FVector UPlayerMouseMoveComponent::GetFixedMoveDirection() const {
		FVector Direction = Rotation.RotateVector(MoveDirection); 
		
		const FVector Temp = Direction;
		Direction.X = -Temp.Y;
		Direction.Y =  Temp.X;

		return Direction;
	}
