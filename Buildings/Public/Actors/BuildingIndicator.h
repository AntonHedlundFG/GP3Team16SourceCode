#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BuildingIndicator.generated.h"

class UPriceDataAsset;
class USceneComponent;
class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class BUILDINGS_API ABuildingIndicator : public AActor
{
	GENERATED_BODY()
	
public:	

	ABuildingIndicator();
	
	const bool IsObstructed(const FVector& Location);

protected:

	UPROPERTY(Category="Indicator", EditDefaultsOnly, BlueprintReadOnly)
	TEnumAsByte<ECollisionChannel> ObstructionChannel = ECC_Visibility; 

	UPROPERTY(Category="Indicator", EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UMaterial> NormalMaterial = nullptr;

	UPROPERTY(Category="Indicator", EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UMaterial> ObstructedMaterial = nullptr;

	
	virtual void BeginPlay() override;

private:

	bool bObstructed = false;
	FQuat ActorRotation = FQuat::Identity;
	FBoxSphereBounds CollisionBounds = FBoxSphereBounds();
	
	UPROPERTY() UWorld* World = nullptr;
	UPROPERTY() UBoxComponent* BoxCollision = nullptr;
	UPROPERTY() TArray<UStaticMeshComponent*> StaticMeshes;
	
	void InitBoxCollision();
	void InitStaticMesh();
	
};
