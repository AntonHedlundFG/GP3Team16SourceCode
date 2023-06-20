#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EResourceType : uint8 {
	RT_UNSET	= 0 UMETA(DisplayName="Unspecified"),
	RT_FOOD 	= 1 UMETA(DisplayName="Food"),
	RT_MATERIAL = 2 UMETA(DisplayName="Build Material"),
	RT_HOUSING	= 3	UMETA(DisplayName="Housing"),
	RT_EGG		= 4 UMETA(DisplayName="Eggs")
};
