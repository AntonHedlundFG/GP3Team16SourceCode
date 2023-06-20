#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InputBindableInterface.generated.h"

UINTERFACE(MinimalAPI)
class UInputBindableInterface : public UInterface {
	GENERATED_BODY()
};

class UTILITIES_API IInputBindableInterface
{
	GENERATED_BODY()

public:

	virtual void BindInput(UEnhancedInputComponent* InputComponent)   = 0;
	virtual void UnbindInput(UEnhancedInputComponent* InputComponent) = 0;
	
};
