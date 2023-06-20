#pragma once

#include "CoreMinimal.h"

#if WITH_EDITOR

	#define LOG_SCREEN(Text, Colour, Time) LogToScreen(Text, Colour, Time);

	static void LogToScreen(const FString& Text, const FColor& Colour, const float Time = 5){
		GEngine->AddOnScreenDebugMessage(
			-1, Time, Colour, Text
		);
	};

#else

	#define LOG_SCREEN(Text, Colour, Time)

#endif