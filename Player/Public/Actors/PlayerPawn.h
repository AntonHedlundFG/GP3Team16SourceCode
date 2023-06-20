#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UEnhancedInputComponent;
class UInputMappingContext;

UCLASS()
class PLAYER_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	
	APlayerPawn();
	
	UEnhancedInputComponent* GetEnhancedInput() const { return EnhancedInput; };
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	UPROPERTY(Category="Player", EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UInputMappingContext> PlayerMapping = nullptr;

	
	virtual void BeginPlay() override;
	
private:
	
	UPROPERTY(EditAnywhere) UCameraComponent* Camera = nullptr;
	UPROPERTY(EditAnywhere) USpringArmComponent* SpringArm = nullptr;
	UPROPERTY(EditAnywhere) UEnhancedInputComponent* EnhancedInput = nullptr;

	
	void InitSpringArm();
	void InitCamera();
	void InitInputMapping();
	
};
