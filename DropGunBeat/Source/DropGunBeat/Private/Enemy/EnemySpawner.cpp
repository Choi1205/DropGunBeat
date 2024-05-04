// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemySpawner.h"
#include "Components/BoxComponent.h"
#include "Enemy/BaseEnemy.h"
#include "GunPlayer.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(rootComp);

	spawnCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("spawnCollision"));
	spawnCollision->SetupAttachment(rootComp);
	spawnCollision->SetBoxExtent(FVector(10.0f, 100.0f, 100.0f));

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
	spawnCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemySpawner::SpawnEnemy);
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpawner::SpawnEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<AGunPlayer>()) {
		if (!bIsSpawned) {
			FActorSpawnParameters params;
			params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			for (int32 i = 0; i < spawnPoint.Num(); i++) {
				ABaseEnemy* enemy = GetWorld()->SpawnActor<ABaseEnemy>(enemy_BP, spawnPoint[i], FRotator(0.0f, 180.0f, 0.0f), params);
				enemy->SetTargetPlace(movePoint[i]);
			}
			bIsSpawned = true;
		}
	}
}

