#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/SelectableUnitInterface.h"
#include "BuildingActor.generated.h"

class USceneComponent;
class UBoxComponent;
class UStaticMeshComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelectedChangedState, bool, State);

UCLASS()
class BUILDINGS_API ABuildingActor : public AActor, public ISelectableUnitInterface
{
	GENERATED_BODY()
	
public:

	ABuildingActor();

	EUnitSelectionPriority GetSelectionPriority() override { return SelectionPriority; }
	bool IsGroupSelectable() override { return false; }
	
	void SetSelected(bool bSelected) override;
	void AssignTarget(FVector TargetWorldPosition, TArray<TObjectPtr<AActor>> Targets) override {};
	void SetPrimarySelected(bool bPrimarySelected) override;
	
protected:

	UPROPERTY(Category="Building", EditDefaultsOnly, BlueprintReadOnly)
	EUnitSelectionPriority SelectionPriority;

	UPROPERTY(BlueprintAssignable)
	FOnSelectedChangedState OnSelectedStateChange;
	
	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleDefaultsOnly) USceneComponent* SceneRoot = nullptr;
	UPROPERTY(VisibleDefaultsOnly) UBoxComponent* BoxCollision = nullptr;
	UPROPERTY(VisibleDefaultsOnly) UStaticMeshComponent* StaticMesh = nullptr;

	void InitSceneRoot();
	void InitBoxCollision();
	void InitStaticMesh();
	
};
