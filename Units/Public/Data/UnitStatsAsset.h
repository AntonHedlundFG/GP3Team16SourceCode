// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "UnitStatsAsset.generated.h"


class UPaperSprite;

UENUM(BlueprintType)
enum class EUnitStatType : uint8
{
	UST_Damage,
	UST_Range,
	UST_MoveSpeed,
	UST_MaxHealth,
	UST_HousingNeed
};

UCLASS(BlueprintType)
class UNITS_API UUnitStatsAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	FString GetUnitName() { return UnitName; }

	UFUNCTION(BlueprintCallable)
	float GetDamage();

	UFUNCTION(BlueprintCallable)
	float GetRange();

	UFUNCTION(BlueprintCallable)
	float GetMoveSpeed();

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth();

	UFUNCTION(BlueprintCallable)
	UPaperSprite* GetUnitIcon() const { return Icon; }
	
	UPROPERTY(Transient, VisibleAnywhere)
	int DamageTier = 0;

	UPROPERTY(Transient, VisibleAnywhere)
	int RangeTier = 0;

	UPROPERTY(Transient, VisibleAnywhere)
	int MoveSpeedTier = 0;

	UPROPERTY(Transient, VisibleAnywhere)
	int MaxHealthTier = 0;

protected:

	UPROPERTY(EditAnywhere)
	FString UnitName;
		
	UPROPERTY(EditAnywhere)
	TArray<float> Damage;

	UPROPERTY(EditAnywhere)
	TArray<float> Range;

	UPROPERTY(EditAnywhere)
	TArray<float> MoveSpeed;

	UPROPERTY(EditAnywhere)
	TArray<float> MaxHealth;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UPaperSprite> Icon;
	
	float GetTieredValue(int Tier, TArray<float>& InArray);
	
};
