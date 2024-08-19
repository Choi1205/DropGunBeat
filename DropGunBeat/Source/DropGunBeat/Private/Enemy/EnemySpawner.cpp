// Fill out your copyright notice in the Description page of Project Settings.
//플레이어가 박스 컴포넌트에 충돌하면 루트 컴포넌트 위치에 에너미가 생성되는 액터
//언리얼에서 생성할 에너미와 이동할 위치를 지정한다.
//에너미가 생성될 위치에 배치하고, 에너미가 생성될 시점의 플레이어 위치에 박스컴포넌트를 옮겨서 사용한다.

#include "Enemy/EnemySpawner.h"
#include "Components/BoxComponent.h"
#include "Enemy/BaseEnemy.h"
#include "GunPlayer.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// 단순히 주석처리하면 Tick이 안꺼진다? bCanEverTick:1이니까?
	PrimaryActorTick.bCanEverTick = false;

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
//void AEnemySpawner::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//}

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

