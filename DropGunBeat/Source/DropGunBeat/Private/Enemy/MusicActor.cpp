// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/MusicActor.h"
#include "Components/AudioComponent.h"

// Sets default values
AMusicActor::AMusicActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(rootComp);

	audioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	audioComp->SetupAttachment(rootComp);

}

// Called when the game starts or when spawned
void AMusicActor::BeginPlay()
{
	Super::BeginPlay();
	beatTime = 60.0f / BPM;
	currentTime = currentTime + (beatTime/4);
}

// Called every frame
void AMusicActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	currentTime += DeltaTime;

}

float AMusicActor::GetBeatTime()
{
	return beatTime;
}

float AMusicActor::BeatAccuracy()
{
	float vaule = (currentTime / beatTime) * 100;
	int32 v = ((int32)vaule) % 100;
	float fv = (float)v / 100.0f;
	UE_LOG(LogTemp, Warning, TEXT("fv : %f"), fv);
	return FMath::Abs(fv - 0.5f);
}

