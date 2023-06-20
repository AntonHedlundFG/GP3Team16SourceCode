#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResourceActor.generated.h"

class UResourceDataAsset;

UCLASS(Blueprintable)
class RESOURCES_API AResourceActor : public AActor
{
	GENERATED_BODY()

public:

	const UResourceDataAsset* GetResourceSettings() const {
		return ResourceSettings;
	}
	
	AResourceActor();
	void Hide(const bool Hidden) const;

protected:

	UPROPERTY(Category="Resource", EditDefaultsOnly, BlueprintReadOnly)
	UResourceDataAsset* ResourceSettings = nullptr;

	
	virtual void BeginPlay() override;
	
private:

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh = nullptr;

	
	void InitStaticMesh();

};