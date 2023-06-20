// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Enums/SelectorTargetType.h"
#include "SelectorTargetInterface.generated.h"


// UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
// enum class ESelectorTargetType : uint8 {
// 	STT_NONE = 0			UMETA(Hidden),
// 	STT_HOSTILE_UNIT = 1	UMETA(DisplayName = "Hostile Unit"),
// 	STT_RESOURCE = 2		UMETA(DisplayName = "Resource")
// };
//ENUM_CLASS_FLAGS(ESelectorTargetType);

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class USelectorTargetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UTILITIES_API ISelectorTargetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	ESelectorTargetType GetSelectorType();

};
