#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerCursorComponent.generated.h"

class ABuildingActor;
class ADirectControlledUnitActor;
class APlayerController;

UCLASS(ClassGroup="Player", meta=(BlueprintSpawnableComponent))
class PLAYER_API UPlayerCursorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UPlayerCursorComponent();

	UFUNCTION(BlueprintImplementableEvent)
	void OnMouseOver(ADirectControlledUnitActor* Unit);
	
protected:

	UPROPERTY(Category="Player", EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<ECollisionChannel> SelectionChannel = ECC_WorldDynamic;

	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	UPROPERTY() ABuildingActor* SelectedBuilding = nullptr;
	UPROPERTY() ADirectControlledUnitActor* SelectedUnit = nullptr;
	UPROPERTY() APlayerController* PlayerController = nullptr;
	UPROPERTY() UWorld* World = nullptr;

	
	void InitPlayerController();
	
	const void GetTracePositions(FVector& Start, FVector& End) const;
	const bool SelectedActor(const FHitResult& HitResult);
	const FHitResult GetMouseHitResult(const FVector& Start, const FVector& End, const ECollisionChannel CollisionChannel) const;
	
};
