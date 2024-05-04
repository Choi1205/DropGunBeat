// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MusicActor.generated.h"

UCLASS()
class DROPGUNBEAT_API AMusicActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMusicActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* rootComp;
	UPROPERTY(EditDefaultsOnly)
	class UAudioComponent* audioComp;

	UPROPERTY(EditDefaultsOnly)
	float BPM;
	float beatTime;

	float currentTime = 0.0f;

	bool beat;

	UFUNCTION(BlueprintCallable)
	float GetBeatTime();

	UFUNCTION(BlueprintCallable)
	float BeatAccuracy();
};
