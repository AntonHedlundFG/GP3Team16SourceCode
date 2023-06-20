#include "Actors/PlayerPawn.h"
#include "Utilities/Public/Interfaces/InputBindableInterface.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

// CONSTRUCTION

	APlayerPawn::APlayerPawn(){
		PrimaryActorTick.bStartWithTickEnabled = false;
		PrimaryActorTick.bCanEverTick = false;

		//InitSpringArm();
		//InitCamera();
	}

	void APlayerPawn::InitSpringArm(){
		auto* Root = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
		SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));

		SpringArm->SetRelativeLocation(FVector::Zero());
		SpringArm->SetRelativeRotation(FRotator(-45, 0, 0));
		SpringArm->SetupAttachment(Root);

		SpringArm->bDoCollisionTest = false;
	}

	void APlayerPawn::InitCamera(){
		if (!SpringArm) return;
		Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
		Camera->SetupAttachment(SpringArm);

		Camera->SetRelativeLocation(FVector::Zero());
		Camera->SetRelativeRotation(FRotator::ZeroRotator);
	}

// INITIALISATION

	void APlayerPawn::BeginPlay(){
		Super::BeginPlay();
		InitInputMapping();
	}

// INPUT HANDLING

	void APlayerPawn::InitInputMapping(){
		const auto* PlayerController = Cast<APlayerController>(GetController());
		if (!PlayerController) UE_LOG(LogTemp, Fatal, TEXT("Failed Cast: PlayerController"));

		const auto* LocalPlayer = PlayerController->GetLocalPlayer();
		if (!LocalPlayer) UE_LOG(LogTemp, Fatal, TEXT("Failed Get: LocalPlayer"));

		auto* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		if (!InputSystem) UE_LOG(LogTemp, Fatal, TEXT("Failed Get: EnhancedInputLocalPlayerSubsystem"));
		
		if (PlayerMapping.IsNull()) UE_LOG(LogTemp, Fatal, TEXT("Missing Reference: PlayerMapping"));
		InputSystem->AddMappingContext(PlayerMapping.LoadSynchronous(), 0);
	}

	void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent){
		Super::SetupPlayerInputComponent(PlayerInputComponent);
		EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
		
		const auto Components = GetComponentsByInterface(
			UInputBindableInterface::StaticClass()
		);

		for (int i = 0; i < Components.Num(); ++i){
			auto* Bindable = Cast<IInputBindableInterface>(Components[i]);
			if (Bindable) Bindable->BindInput(EnhancedInput);
		}
	}
