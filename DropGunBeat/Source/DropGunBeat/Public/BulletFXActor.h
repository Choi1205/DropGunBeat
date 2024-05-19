// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletFXActor.generated.h"

UCLASS()
class DROPGUNBEAT_API ABulletFXActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABulletFXActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, Category = "MySettings")
	class USceneComponent* rootComp;

	UPROPERTY(VisibleAnywhere, Category = "MySettings")
	class UParticleSystemComponent* particleComp;

	void PlayFX();

};
