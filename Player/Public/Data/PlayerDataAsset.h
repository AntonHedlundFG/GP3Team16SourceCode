#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlayerDataAsset.generated.h"

UCLASS()
class PLAYER_API UPlayerDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	// Gameplay Settings
	FVector2D& GetHomeLocation(){ return HomeLocation; }
	FVector2D& GetHomeExtent(){ return HomeExtent; }
	const float GetMaxCheckPointTime() const { return MaxCheckPointTime; }
	
	// Movement Settings
	const FVector2D GetMinMaxMoveSpeed() const { return MinMaxMoveSpeed; }
	const float GetKeysMoveSpeedMultiplier() const { return KeysMoveSpeedMultiplier; }
	const float GetScreenMoveSpeedMultiplier() const { return ScreenMoveSpeedMultiplier; }
	const float GetDragMoveSpeedMultiplier() const { return DragMoveSpeedMultiplier; }
	const float GetScreenBoarderPixelSize() const { return ScreenBoarderPixelSize; }
	
	// Camera Settings
	const float GetZoomSpeed() const { return ZoomSpeed; }
	const float GetStartingZoomDistance() const { return StartingZoomDistance; }
	const FVector2D GetMinMaxZoomDistance() const { return MinMaxZoomDistance; }
	
protected:
	
	UPROPERTY(Category="Gameplay", EditAnywhere, BlueprintReadWrite)
	FVector2D HomeLocation = FVector2D::Zero();

	UPROPERTY(Category="Gameplay", EditAnywhere, BlueprintReadWrite)
	FVector2D HomeExtent = FVector2D(1000, 1000);

	UPROPERTY(Category="Gameplay", EditAnywhere, BlueprintReadWrite, meta=(ToolTip = "Time in Seconds"))
	float MaxCheckPointTime = 60;
	
	
	UPROPERTY(Category="Movement", EditDefaultsOnly, BlueprintReadOnly)
	FVector2D MinMaxMoveSpeed = FVector2D(1500, 3500);

	UPROPERTY(Category="Movement", EditDefaultsOnly, BlueprintReadOnly)
	float KeysMoveSpeedMultiplier = 1;

	UPROPERTY(Category="Movement", EditDefaultsOnly, BlueprintReadOnly)
	float ScreenMoveSpeedMultiplier = 1;
	
	UPROPERTY(Category="Movement", EditDefaultsOnly, BlueprintReadOnly)
	float DragMoveSpeedMultiplier = 1;
	
	UPROPERTY(Category="Movement", EditDefaultsOnly, BlueprintReadOnly)
	float ScreenBoarderPixelSize = 20.f;

	
	UPROPERTY(Category="Camera", EditDefaultsOnly, BlueprintReadOnly)
	float ZoomSpeed = 150.f;

	UPROPERTY(Category="Camera", EditDefaultsOnly, BlueprintReadOnly)
	float StartingZoomDistance = 1500.f;
		
	UPROPERTY(Category="Camera", EditDefaultsOnly, BlueprintReadOnly)
	FVector2D MinMaxZoomDistance = FVector2D(-2500, 3500);

};