#pragma once

UENUM(BlueprintType)
enum ESoundPriority
{
	SP_UNSPECIFIED	UMETA(DisplayName = "Unspecified"),

	// Always played independent of camera location 
	SP_WARNING		UMETA(DisplayName = "Warning"),
	SP_ALERT		UMETA(DisplayName = "Alert"),
	SP_PICKUP		UMETA(DisplayName = "Pickup"),

	// Played only when its within camera view
	SP_PURCHASE		UMETA(DisplayName = "Purchase"),
	SP_COMBAT		UMETA(DisplayName = "Combat"),
	SP_VOICELINES	UMETA(DisplayName = "Voicelines"),

	// Played at full volume only when no other sound is played
	SP_BACKGROUND	UMETA(DisplayName = "Background"),
	
};
