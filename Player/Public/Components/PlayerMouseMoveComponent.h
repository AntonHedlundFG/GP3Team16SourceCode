#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerMouseMoveComponent.generated.h"

class UPlayerVelocityComponent;
class USpringArmComponent;
class UPlayerDataAsset;

UCLASS(ClassGroup="Player", meta=(BlueprintSpawnableComponent))
class PLAYER_API UPlayerMouseMoveComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UPlayerMouseMoveComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	UPROPERTY(Category="Player", EditDefaultsOnly, BlueprintReadOnly)
	UPlayerDataAsset* PlayerSettings = nullptr;

	
	virtual void BeginPlay() override;
	
private:

	bool bMoving = false;
	bool bTicking = false;
	
	FVector MoveDirection = FVector::Zero();
	FVector2D MousePosition = FVector2D::Zero();
	FRotator Rotation = FRotator::ZeroRotator;
	
	UPROPERTY() AActor* Owner = nullptr;
	UPROPERTY() APlayerController* PlayerController = nullptr;
	UPROPERTY() UGameViewportClient* Viewport = nullptr;
	UPROPERTY() UPlayerVelocityComponent* VelocityComponent = nullptr;
	UPROPERTY() USpringArmComponent* SpringArmComponent = nullptr;

	
	void InitSpringArmComponent();
	void InitVelocityComponent();
	
	const bool ValidMoveDirection();
	const float GetMoveSpeed() const;
	FVector GetFixedMoveDirection() const;
	
};