// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/UnitStatsAsset.h"

float UUnitStatsAsset::GetDamage()
{
	return GetTieredValue(DamageTier, Damage);
}

float UUnitStatsAsset::GetRange()
{
	return GetTieredValue(RangeTier, Range);
}

float UUnitStatsAsset::GetMoveSpeed()
{
	return GetTieredValue(MoveSpeedTier, MoveSpeed);
}

float UUnitStatsAsset::GetMaxHealth()
{
	return GetTieredValue(MaxHealthTier, MaxHealth);
}

float UUnitStatsAsset::GetTieredValue(int Tier, TArray<float>& InArray)
{
	return InArray.Num() ? InArray[FMath::Min(Tier, InArray.Num())] : 0.0f;
}