#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CollidableInterface.generated.h"

class ICollisionHandlerInterface;

UINTERFACE(MinimalAPI)
class UCollidableInterface : public UInterface {
	GENERATED_BODY()
};

class PLAYER_API ICollidableInterface
{
	GENERATED_BODY()

public:

	virtual void SetHandler(ICollisionHandlerInterface* Handler) = 0;
	virtual void SetObstructed(const bool Obstructed) = 0;
	
};
