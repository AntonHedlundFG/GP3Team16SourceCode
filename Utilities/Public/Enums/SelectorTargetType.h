#pragma once

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ESelectorTargetType : uint8 {
	STT_NONE = 0			UMETA(Hidden),
	STT_HOSTILE_UNIT = 1	UMETA(DisplayName = "Hostile Unit"),
	STT_RESOURCE = 2		UMETA(DisplayName = "Resource")
};
ENUM_CLASS_FLAGS(ESelectorTargetType);
