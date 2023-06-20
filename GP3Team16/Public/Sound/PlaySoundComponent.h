#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlaySoundComponent.generated.h"

class ASoundManager;
class USoundDataAsset;
enum ESoundPriority;

UCLASS(ClassGroup="Sound", meta=(BlueprintSpawnableComponent))
class GP3TEAM16_API UPlaySoundComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UPlaySoundComponent();

	UFUNCTION(BlueprintCallable)
	void PlaySound(const uint8 Index);
	
protected:

	UPROPERTY(Category="Sound", EditDefaultsOnly, BlueprintReadOnly)
	TArray<TObjectPtr<USoundDataAsset>> Sounds;
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
private:

	TArray<FIntVector2> PlayCount;
	FVector SoundLocation = FVector::Zero();
	TObjectPtr<USoundDataAsset> CurrentSound = nullptr;
	
	UPROPERTY() AActor* Owner = nullptr;
	UPROPERTY() ASoundManager* SoundManager = nullptr;
	UPROPERTY() UAudioComponent* AudioComponent = nullptr;

	
	void InitAudioComponent();
	void InitSoundManager();

	void PlaySoundAsset(USoundDataAsset* Sound, FIntVector2& PlayCountAndThreshold);
	
};
