// Fill out your copyright notice in the Description page of Project Settings.
// 플레이어가 충돌하면 데미지를 입히는 기둥 액터

#include "Enemy/DamagePillar.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GunPlayer.h"

// Sets default values
ADamagePillar::ADamagePillar()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(rootComp);

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("spawnCollision"));
	boxComp->SetupAttachment(rootComp);
	boxComp->SetBoxExtent(FVector(40.0f, 40.0f, 50.0f));
	boxComp->SetCollisionProfileName(FName("OverlapAllDynamic"));

	pillarShape = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PillarMesh"));
	pillarShape->SetupAttachment(boxComp);
	pillarShape->SetCollisionProfileName(FName("NoCollision"));

}

// Called when the game starts or when spawned
void ADamagePillar::BeginPlay()
{
	Super::BeginPlay();
	
	boxComp->OnComponentBeginOverlap.AddDynamic(this, &ADamagePillar::OnOverlaped);

	//bIsNeedRepete는 연속배치용 변수. 언리얼에서 값을 변경해서 사용한다.
	//true인 경우 배치된 기둥 뒤로 같은 크기의 기둥을 저정갯수, 지정간격으로 연속배치한다.
	if (bIsNeedRepete) {
		spawnLoc = GetActorLocation();
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		for(int32 i = 1; i <= repeteTime; i++){
			spawnLoc += FVector(repeteDistance, 0.0f, 0.0f);
			ADamagePillar* pillar = GetWorld()->SpawnActor<ADamagePillar>(pillarFactory, spawnLoc, GetActorRotation(), params);
			pillar->SetActorScale3D(GetActorScale3D());
		}
	}
}

// Called every frame
//void ADamagePillar::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

//충돌시 데미지 처리
void ADamagePillar::OnOverlaped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AGunPlayer* playerREF = Cast<AGunPlayer>(OtherActor);
	if (!bIsDamaged && playerREF != nullptr) {
		bIsDamaged = true;
		playerREF->OnDamaged();
		GetWorldTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda([&]() {
			Destroy();
			}));
	}
}

