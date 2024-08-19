// Fill out your copyright notice in the Description page of Project Settings.
//���� ��� �� 1��Ʈ �ð�, ��Ȯ���� ����ϴ� ����

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
	//BPM�� �ܺ� ���α׷��� ���ؼ� ����ϰ�, �𸮾󿡼� �Է��Ѵ�.
	beatTime = 60.0f / BPM;
}

// Called every frame
void AMusicActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//������ ����Ǹ� DeltaTime�� �����ؼ� ���� ����ð��� ����Ѵ�.
	if(bIsMusicStart) {
		currentTime += DeltaTime;
	}
	//�÷��̾ �°��� ���� ������ ���󺹱��Ѵ�.
	if (bIsVolumeUp) {
		float hitDeltaTime = FMath::Clamp(currentTime - hitTime, 0.0f, 3.0f);
		audioComp->AdjustVolume(0.01f, FMath::Lerp(0.001f, 1.0f, hitDeltaTime / volumeUpTime), EAudioFaderCurve::Linear);
		if (hitDeltaTime == 3.0f) {
			bIsVolumeUp = false;
		}
	}
}

//1��Ʈ�� �ð��� ����
float AMusicActor::GetBeatTime()
{
	return beatTime;
}

//���� ����ð��� 1��Ʈ�� �ð����� ���� �������� 1��Ʈ�� �ð��� ������ ��������� 50%�� ���� ��Ʈ�� ���� �ʴ�.
//����/���� ó���� ���� �ʰ� �����Ƿ�, ��������� 50%�� �� ���� ���밪�� 0�� ����� ���� ��Ʈ�� ���� �ʴ�.
float AMusicActor::BeatAccuracy()
{
	//beatTime = 60.0f / BPM;
	float vaule = (currentTime / beatTime) * 100;
	int32 v = ((int32)vaule) % 100;
	float fv = (float)v / 100.0f;
	UE_LOG(LogTemp, Warning, TEXT("fv : %f"), fv);
	return FMath::Abs(fv - 0.5f);
}

//���� ��Ʈ������ �Ϸ�Ǹ� ����� ���۵ȴ�.
void AMusicActor::MusicStart()
{
	audioComp->Activate(true);
	bIsMusicStart = true;
}

//�÷��̾ ���ݹ����� �Ͻ������� ���� ������ 0�� ������ ���δ�.
//0���� ���̸� ������ �߻��Ѵ�.
void AMusicActor::VolumeHitReact(bool bIsDead)
{
	audioComp->AdjustVolume(0.01f, 0.001f, EAudioFaderCurve::Linear);
	if (!bIsDead) {
		bIsVolumeUp = true;
		hitTime = currentTime;
	}
}

