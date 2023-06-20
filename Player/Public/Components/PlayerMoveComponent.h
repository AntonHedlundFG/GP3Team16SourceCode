#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/InputBindableInterface.h"
#include "PlayerMoveComponent.generated.h"

class USpringArmComponent;
class UPlayerVelocityComponent;
class UPlayerDataAsset;
class UInputAction;
struct FInputActionInstance;
struct FEnhancedInputActionEventBinding;

UCLASS(ClassGroup="Player", meta=(BlueprintSpawnableComponent))
class PLAYER_API UPlayerMoveComponent : public UActorComponent,
public IInputBindableInterface
{
	GENERATED_BODY()

public:
	
	UPlayerMoveComponent();

	virtual void BindInput(UEnhancedInputComponent* InputComponent) override;
	virtual void UnbindInput(UEnhancedInputComponent* InputComponent) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	UPROPERTY(Category="Player", EditDefaultsOnly, BlueprintReadOnly)
	UPlayerDataAsset* PlayerSettings = nullptr;
	
	UPROPERTY(Category="Player", EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* MoveInput = nullptr;

	
	virtual void BeginPlay() override;

private:
	
	bool bMoving = false;
	bool bTicking = false;

	FVector2D InputDirection = FVector2d::Zero();
	FVector MoveDirection = FVector::Zero();
	FRotator Rotation = FRotator::ZeroRotator;

	TObjectPtr<FEnhancedInputActionEventBinding> InputBindingTriggered;
	TObjectPtr<FEnhancedInputActionEventBinding> InputBindingCompleted;

	UPROPERTY() AActor* Owner = nullptr;
	UPROPERTY() UPlayerVelocityComponent* VelocityComponent = nullptr;
	UPROPERTY() USpringArmComponent* SpringArmComponent = nullptr;

	
	void InitSpringArmComponent();
	void InitVelocityComponent();
	
	void OnMoveInput(const FInputActionInstance& Instance);
	void OnMove(const FVector2D& Direction);
	const float GetMoveSpeed() const;

};