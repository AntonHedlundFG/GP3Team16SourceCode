#include "Sound/SoundManager.h"
#include "Camera/CameraComponent.h"
#include "Sound/SoundPriority.h"
#include "Kismet/GameplayStatics.h"

// INITIALISATION

	ASoundManager::ASoundManager(){
		PrimaryActorTick.bStartWithTickEnabled = false;
		PrimaryActorTick.bCanEverTick = false;
	}

	void ASoundManager::BeginPlay(){
		Super::BeginPlay();

		InitPlayerController();
		InitCameraManager();
		InitPlayerCamera();
	}

	void ASoundManager::InitPlayerController(){
		PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

		if (!PlayerController) UE_LOG(LogTemp, Fatal,
			TEXT("Missing Reference: PlayerController")
		);
	}

	void ASoundManager::InitCameraManager(){
		PlayerCameraManager = PlayerController->PlayerCameraManager;
		
		if (!PlayerCameraManager) UE_LOG(LogTemp, Fatal,
			TEXT("Missing Reference: PlayerCameraManager")
		);
	}

	void ASoundManager::InitPlayerCamera(){
		PlayerCameraActor = PlayerController->GetViewTarget();
		
		if (!PlayerCameraActor) UE_LOG(LogTemp, Fatal,
			TEXT("Missing Reference: CameraActor")
		);
		PlayerCamera = Cast<UCameraComponent>(
			PlayerCameraActor->GetComponentByClass(UCameraComponent::StaticClass())
		);
		if (!PlayerCamera) UE_LOG(LogTemp, Fatal,
			TEXT("Missing Reference: CameraComponent")
		);
	}

// SOUND HANDLING

	const bool ASoundManager::IsHighPriority(const ESoundPriority Priority) const {
		switch (Priority){
			case SP_WARNING:
			case SP_ALERT:
			case SP_PICKUP:
				return true;
			
			case SP_PURCHASE:
			case SP_COMBAT:
			case SP_VOICELINES:
			case SP_UNSPECIFIED:
			case SP_BACKGROUND:
				return false;
		}
		return false;
	}

	const bool ASoundManager::RequestToPlaySound(const ESoundPriority Priority, const FVector& Location) const {
		if (IsHighPriority(Priority))
			return true;

		return ValidatePriorityRequest(Location);
	}

	const bool ASoundManager::ValidatePriorityRequest(const FVector& SoundLocation) const {
		const FVector CameraLocation = PlayerCameraActor->GetActorLocation();
		
		const FVector SoundPoint = FVector(SoundLocation.X, SoundLocation.Y, CameraLocation.Z);
		const float   Distance   = FVector::DistSquared(CameraLocation, SoundPoint);

		// FIX THIS, NEEDS TO BE CALCULATED BASED ON CAMERA FRUSTUM AND ZOOM LEVEL
		return Distance < 5000;
	}

	// REWORK ME
	const float ASoundManager::GetVolume(const ESoundPriority Priority, const FVector& Location) const {
		switch (Priority){
			case SP_WARNING: case SP_ALERT: case SP_PICKUP: {
				return 1;
			}
			case SP_PURCHASE: case SP_COMBAT: case SP_VOICELINES: {
				return 0.5f;
			}
			case SP_UNSPECIFIED: case SP_BACKGROUND: {
				return 0.25f;
			}
		}
		return 0;
	}

// FRUSTUM HANDLING

	void ASoundManager::Tick(float DeltaSeconds){
		Super::Tick(DeltaSeconds);

		// FIND DISTANCE AND ORIGIN
		// CALCULATE FRUSTUM
	}