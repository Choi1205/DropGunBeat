// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

UCLASS()
class DROPGUNBEAT_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* rootComp;

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* spawnCollision;

	UPROPERTY(EditAnywhere, Category = "SpawnSetting")
	TSubclassOf<class ABaseEnemy> enemy_BP;

	UPROPERTY(EditAnywhere, Category = "SpawnSetting")
	TArray<FVector> spawnPoint;

	UPROPERTY(EditAnywhere, Category = "SpawnSetting")
	TArray<FVector> movePoint;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	bool bIsSpawned = false;

	UFUNCTION()
	void SpawnEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
