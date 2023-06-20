#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CollisionHandlerInterface.generated.h"

class ICollidableInterface;

UINTERFACE(MinimalAPI)
class UCollisionHandlerInterface : public UInterface{
	GENERATED_BODY()
};

class PLAYER_API ICollisionHandlerInterface
{
	GENERATED_BODY()

public:

	virtual void SetCollisionState(ICollidableInterface* Collidable,
		const bool CheckCollision, FVector Direction) = 0;
	
};
