#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/SelectorTargetInterface.h"
#include "ResourceStockpileActor.generated.h"

class UBoxComponent;
class AResourceActor;
class UResourceDataAsset;

/* Deprecated: Use BP_ResourceStockpile instead */
UCLASS(meta=(DeprecationMessage="Use BP_ResourceStockpile instead"))
class RESOURCES_API AResourceStockpileActor : public AActor, public ISelectorTargetInterface
{
	GENERATED_BODY()

public:

	AResourceStockpileActor();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	ESelectorTargetType GetSelectorType(); virtual ESelectorTargetType GetSelectorType_Implementation() override {
		return ESelectorTargetType::STT_RESOURCE;
	}

	UFUNCTION(BlueprintCallable)
	void AssignResource(const AActor* Worker);
	
	UFUNCTION(BlueprintImplementableEvent)
	void OnAssignedResource(const AActor* Worker, const AResourceActor* Resource);

	UFUNCTION(BlueprintCallable)
	void PickedUpResource(const AResourceActor* AssignedResource);

	UFUNCTION(BlueprintCallable)
	void AbandonedResource(AResourceActor* AssignedResource);

protected:

	UPROPERTY(Category="Resources", EditAnywhere, BlueprintReadWrite)
	uint8 ResourceCount = 1;

	UPROPERTY(Category="Resources", EditAnywhere, BlueprintReadWrite)
	float ResourcePadding = 50;
	
	UPROPERTY(Category="Resources", EditAnywhere, BlueprintReadWrite)
	UResourceDataAsset* ResourceSettings = nullptr;


	virtual void BeginPlay() override;
	
private:
	
	TArray<AResourceActor*> ActiveResources;
	
	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxTrigger = nullptr;
	

	const bool ValidateReferences() const;
	void InitBoxCollision();
	void InitTriggerOverlap();
	void SpawnResources();

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};