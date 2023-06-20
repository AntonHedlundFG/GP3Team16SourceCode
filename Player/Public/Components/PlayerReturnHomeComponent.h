#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/InputBindableInterface.h"
#include "PlayerReturnHomeComponent.generated.h"

class UPlayerDataAsset;
class UInputAction;
struct FInputActionInstance;
struct FEnhancedInputActionEventBinding;

UCLASS(ClassGroup="Player", meta=(BlueprintSpawnableComponent))
class PLAYER_API UPlayerReturnHomeComponent : public UActorComponent, public IInputBindableInterface
{
	GENERATED_BODY()

public:	
	
	UPlayerReturnHomeComponent();

	virtual void BindInput(UEnhancedInputComponent* InputComponent) override;
	virtual void UnbindInput(UEnhancedInputComponent* InputComponent) override;
	
protected:

	UPROPERTY(Category="Player", EditDefaultsOnly, BlueprintReadOnly)
	UPlayerDataAsset* PlayerSettings = nullptr;
	
	UPROPERTY(Category="Player", EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* HomeInput = nullptr;

	
	virtual void BeginPlay() override;

private:

	float LastReturnTime = 0;
	FVector LastLocation = FVector::Zero();
	
	TObjectPtr<FEnhancedInputActionEventBinding> InputBinding;
	UPROPERTY() AActor* Owner = nullptr;
	

	const bool InsideHome(const FVector& Location, const FVector2D& Home) const;
	const float GetElapsedTime() const;
	
	void OnMoveInput(const FInputActionInstance& Instance);
	void OnReturnHome();
	
};