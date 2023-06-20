#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SoundManager.generated.h"

class UCameraComponent;
class APlayerCameraManager;
enum ESoundPriority;

UCLASS()
class GP3TEAM16_API ASoundManager : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ASoundManager();

	const FVector GetCameraLocation() const {
		return PlayerCameraManager->GetCameraLocation();
	}

	const bool IsHighPriority(const ESoundPriority Priority) const;
	const bool RequestToPlaySound(const ESoundPriority Priority, const FVector& Location) const;
	
	const float GetVolume(const ESoundPriority Priority, const FVector& Location) const;

	
protected:
	
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	
private:

	UPROPERTY() APlayerController* PlayerController = nullptr;
	UPROPERTY() APlayerCameraManager* PlayerCameraManager = nullptr;
	UPROPERTY() UCameraComponent* PlayerCamera = nullptr;
	UPROPERTY() AActor* PlayerCameraActor = nullptr;
	
	void InitPlayerController();
	void InitCameraManager();
	void InitPlayerCamera();

	const bool ValidatePriorityRequest(const FVector& SoundLocation) const;
	
};
