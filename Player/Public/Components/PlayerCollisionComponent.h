#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/CollisionHandlerInterface.h"
#include "PlayerCollisionComponent.generated.h"

class ICollidableInterface;
class UPlayerMouseMoveComponent;
class UPlayerDragMoveComponent;
class UPlayerMoveComponent;

UCLASS(ClassGroup="Player", meta=(BlueprintSpawnableComponent))
class PLAYER_API UPlayerCollisionComponent : public UActorComponent, public ICollisionHandlerInterface
{
	GENERATED_BODY()

public:	
	
	UPlayerCollisionComponent();
	
	virtual void SetCollisionState(ICollidableInterface* Collidable,
		const bool CheckCollision, FVector Direction) override;
	
protected:

	UPROPERTY(Category="Player", EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<ECollisionChannel> CollisionChannel = ECC_WorldStatic;
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
private:

	bool bTicking = false;
	FVector CheckDirection = FVector::Zero();
	
	TArray<bool> CollisionStates;
	TArray<FVector> CollisionDirections;
	TArray<TObjectPtr<ICollidableInterface>> Collidables;

	UPROPERTY() AActor* Owner = nullptr;
	UPROPERTY() UWorld* World = nullptr;

	
	void InitCollidables();

	bool GetTickState();
	FVector GetCheckDirection();
	
};
