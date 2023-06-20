#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/InputBindableInterface.h"
#include "BuildingPlacementComponent.generated.h"

class ABuildingSpawnManager;
class AGP3Team16GameModeBase;
class UInputAction;
struct FEnhancedInputActionEventBinding;
struct FInputActionInstance;

UCLASS(ClassGroup="Buildings", meta=(BlueprintSpawnableComponent))
class BUILDINGS_API UBuildingPlacementComponent : public UActorComponent, public IInputBindableInterface
{
	GENERATED_BODY()

public:	
	
	UBuildingPlacementComponent();

	virtual void BindInput(UEnhancedInputComponent* InputComponent) override;
	virtual void UnbindInput(UEnhancedInputComponent* InputComponent) override;
	
protected:

	UPROPERTY(Category="Player", EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> PlaceBuildingInput = nullptr;

	UPROPERTY(Category="Player", EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> CancelBuildingInput = nullptr;


private:

	TObjectPtr<FEnhancedInputActionEventBinding> PlaceInputBindingCompleted;
	TObjectPtr<FEnhancedInputActionEventBinding> CancelInputBindingCompleted;
	
	
	TObjectPtr<ABuildingSpawnManager> BuildingManager;
	ABuildingSpawnManager* GetBuildingManager();
	
	void OnPlaceBuildingInput(const FInputActionInstance& Instance);
	void OnCancelBuildingInput(const FInputActionInstance& Instance);
		
};
