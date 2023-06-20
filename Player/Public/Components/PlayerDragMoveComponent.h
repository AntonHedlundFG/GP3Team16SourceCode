#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/InputBindableInterface.h"
#include "PlayerDragMoveComponent.generated.h"

class UPlayerVelocityComponent;
class USpringArmComponent;
class UPlayerDataAsset;
class APlayerController;
class UInputAction;
class UEnhancedInputComponent;
struct FEnhancedInputActionEventBinding;
struct FInputActionInstance;

UCLASS(ClassGroup="Player", meta=(BlueprintSpawnableComponent))
class PLAYER_API UPlayerDragMoveComponent : public UActorComponent, public IInputBindableInterface
{
	GENERATED_BODY()

public:
	
	UPlayerDragMoveComponent();

	virtual void BindInput(UEnhancedInputComponent* InputComponent) override;
	virtual void UnbindInput(UEnhancedInputComponent* InputComponent) override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:

	UPROPERTY(Category="Player", EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* DragInput = nullptr;

	UPROPERTY(Category="Player", EditDefaultsOnly, BlueprintReadOnly)
	UPlayerDataAsset* PlayerSettings = nullptr;
	
	UPROPERTY(Category = "Player", EditDefaultsOnly, BlueprintReadOnly, meta = (ClampMin = "0.0", ClampMax = "2.0"))
	float Sensitivity = 0.6f;

	
	virtual void BeginPlay() override;
	
private:

	bool bMoving = false;
	bool bTicking = false;
	
	FVector StartLocation = FVector::Zero();
	FVector MoveDirection = FVector::Zero();
	FVector2D CurrentMousePos  = FVector2D::Zero();
	FVector2D StartingMousePos = FVector2D::Zero();
	FIntPoint StartingMouseViewPortPos;
	FRotator Rotation = FRotator::ZeroRotator;
	
	TObjectPtr<FEnhancedInputActionEventBinding> InputBindingTriggered;
	TObjectPtr<FEnhancedInputActionEventBinding> InputBindingCompleted;
	TObjectPtr<FViewport> ViewPort;
	
	UPROPERTY() AActor* Owner = nullptr;
	UPROPERTY() APlayerController* PlayerController = nullptr;
	UPROPERTY() UPlayerVelocityComponent* VelocityComponent = nullptr;
	UPROPERTY() USpringArmComponent* SpringArmComponent = nullptr;


	void InitSpringArmComponent();
	void InitVelocityComponent();
	void InitViewPort();
	
	void OnDragInput(const FInputActionInstance& Instance);
	void OnDrag(const bool Dragging);

	FVector GetMoveDirection() const;
	const float GetMoveSpeed() const;
	
};