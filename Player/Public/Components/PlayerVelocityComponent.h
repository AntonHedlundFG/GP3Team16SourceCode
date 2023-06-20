#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/CollidableInterface.h"
#include "PlayerVelocityComponent.generated.h"

class UPlayerDataAsset;

UCLASS(ClassGroup="Player", meta=(BlueprintSpawnableComponent))
class PLAYER_API UPlayerVelocityComponent : public UActorComponent, public ICollidableInterface
{
	GENERATED_BODY()

public:	
	
	UPlayerVelocityComponent();

	virtual void SetHandler(ICollisionHandlerInterface* Handler) override;
	virtual void SetObstructed(const bool Obstructed) override;
	
	void AddVelocity(const FVector& Velocity, const float SpeedMultiplier);
	
protected:

	UPROPERTY(Category="Player", EditDefaultsOnly, BlueprintReadOnly)
	UPlayerDataAsset* PlayerSettings = nullptr;

	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	bool bMoving = false;
	bool bTicking = false;
	bool bObstructed = false;
	
	FVector MoveVelocity = FVector::Zero();
	TObjectPtr<ICollisionHandlerInterface> CollisionHandler = nullptr;
	
	UPROPERTY() AActor* Owner = nullptr;
	
};
