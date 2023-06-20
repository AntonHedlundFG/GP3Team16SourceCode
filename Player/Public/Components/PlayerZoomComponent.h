#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/InputBindableInterface.h"
#include "PlayerZoomComponent.generated.h"

class UPlayerDataAsset;
class UInputAction;
class USpringArmComponent;
struct FInputActionInstance;
struct FEnhancedInputActionEventBinding;

UCLASS(ClassGroup="Player", meta=(BlueprintSpawnableComponent))
class PLAYER_API UPlayerZoomComponent : public UActorComponent, public IInputBindableInterface
{
	GENERATED_BODY()

public:	
	
	UPlayerZoomComponent();
	virtual void BindInput(UEnhancedInputComponent* InputComponent) override;
	virtual void UnbindInput(UEnhancedInputComponent* InputComponent) override;
	
protected:
	
	UPROPERTY(Category="Player", EditDefaultsOnly, BlueprintReadOnly)
	UPlayerDataAsset* PlayerSettings = nullptr;
	
	UPROPERTY(Category="Player", EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* ZoomInput = nullptr;


	virtual void BeginPlay() override;
	
private:

	TObjectPtr<FEnhancedInputActionEventBinding> InputBindingTriggered;
	TObjectPtr<FEnhancedInputActionEventBinding> InputBindingCanceled;

	UPROPERTY() USpringArmComponent* SpringArm = nullptr;

	
	void InitSpringArmComponent();
	void InitZoomDistance() const;
	
	void OnZoomInput(const FInputActionInstance& Instance);
	void OnZoom(const float Direction) const;
	
};
