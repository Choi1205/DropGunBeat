// Fill out your copyright notice in the Description page of Project Settings.
// �÷��̾ ��������� �ش� ��ġ�� ���� ����Ʈ�� ����Ѵ�.
// ����ȭ�� �ϱ� ���� �÷��̾ �� ���Ͱ� �÷��̾ ����� ��ġ�� �̵��ؼ� ����Ʈ�� ����Ѵ�.

#include "BulletFXActor.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ABulletFXActor::ABulletFXActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(rootComp);

	//particleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle Component"));
	particleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle Component"));
	particleComp->SetupAttachment(RootComponent);
	//particleComp->SetWorldScale3D(FVector(0.5f));
	particleComp->SetAutoActivate(true);
}

// Called when the game starts or when spawned
//void ABulletFXActor::BeginPlay()
//{
//	Super::BeginPlay();
//	
//}

// Called every frame
//void ABulletFXActor::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

void ABulletFXActor::PlayFX()
{
	particleComp->Activate(true);
}

