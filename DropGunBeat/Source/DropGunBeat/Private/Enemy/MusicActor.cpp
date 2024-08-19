// Fill out your copyright notice in the Description page of Project Settings.
//음악 재생 및 1비트 시간, 정확도를 계산하는 액터

#include "Enemy/MusicActor.h"
#include "Components/AudioComponent.h"
#include <../../../../../../../Source/Runtime/Engine/Public/AudioThread.h>
#include <../../../../../../../Source/Runtime/Engine/Public/AudioDevice.h>
#include <../../../../../../../Source/Runtime/Engine/Public/ActiveSound.h>

// Sets default values
AMusicActor::AMusicActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	rootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(rootComp);

	audioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	audioComp->SetupAttachment(rootComp);
	audioComp->SetAutoActivate(false);
}

// Called when the game starts or when spawned
void AMusicActor::BeginPlay()
{
	Super::BeginPlay();
	//BPM은 외부 프로그램을 통해서 계산하고, 언리얼에서 입력한다.
	beatTime = 60.0f / BPM;
}

// Called every frame
void AMusicActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//음악이 재생되면 DeltaTime를 가산해서 현재 재생시간을 계산한다.
	if(bIsMusicStart) {
		currentTime += DeltaTime;
	}
	//플레이어가 맞고난뒤 음악 볼륨을 원상복구한다.
	if (bIsVolumeUp) {
		float hitDeltaTime = FMath::Clamp(currentTime - hitTime, 0.0f, 3.0f);
		audioComp->AdjustVolume(0.01f, FMath::Lerp(0.001f, 1.0f, hitDeltaTime / volumeUpTime), EAudioFaderCurve::Linear);
		if (hitDeltaTime == 3.0f) {
			bIsVolumeUp = false;
		}
	}
}

//1비트당 시간을 리턴
float AMusicActor::GetBeatTime()
{
	return beatTime;
}

//현재 재생시간을 1비트당 시간으로 나눈 나머지를 1비트당 시간의 비율로 만들었을때 50%가 가장 비트에 맞지 않다.
//빠름/느림 처리는 하지 않고 있으므로, 결과값에서 50%를 뺀 값의 절대값이 0에 가까울 수록 비트에 맞지 않다.
float AMusicActor::BeatAccuracy()
{
	//beatTime = 60.0f / BPM;
	float vaule = (currentTime / beatTime) * 100;
	int32 v = ((int32)vaule) % 100;
	float fv = (float)v / 100.0f;
	UE_LOG(LogTemp, Warning, TEXT("fv : %f"), fv);
	return FMath::Abs(fv - 0.5f);
}

//레벨 스트리밍이 완료되면 재생이 시작된다.
void AMusicActor::MusicStart()
{
	audioComp->Activate(true);
	bIsMusicStart = true;
}

//플레이어가 공격받으면 일시적으로 음악 볼륨을 0에 가깝게 줄인다.
//0으로 줄이면 에러가 발생한다.
void AMusicActor::VolumeHitReact(bool bIsDead)
{
	audioComp->AdjustVolume(0.01f, 0.001f, EAudioFaderCurve::Linear);
	if (!bIsDead) {
		bIsVolumeUp = true;
		hitTime = currentTime;
	}
}

