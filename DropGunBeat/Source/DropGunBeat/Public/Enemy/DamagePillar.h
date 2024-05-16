// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamagePillar.generated.h"

UCLASS()
class DROPGUNBEAT_API ADamagePillar : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADamagePillar();

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* rootComp;

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* boxComp;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* pillarShape;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	bool bIsDamaged = false;

	UFUNCTION()
	void OnOverlaped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ADamagePillar> pillarFactory;
	UPROPERTY(EditAnywhere)
	bool bIsNeedRepete = false;
	UPROPERTY(EditAnywhere)
	int32 repeteTime = 0;
	UPROPERTY(EditAnywhere)
	float repeteDistance;
	FVector spawnLoc;
};
