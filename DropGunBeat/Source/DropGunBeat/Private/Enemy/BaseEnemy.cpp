// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BaseEnemy.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/LineBatchComponent.h"
#include "EngineUtils.h"
#include "GunPlayer.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/Camera/CameraComponent.h>
#include <../../../../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h>
#include "BulletActor.h"
#include "Enemy/MusicActor.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -88.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, 0.0f, -90.0f));

	vestMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Vest"));
	vestMeshComp->SetupAttachment(GetMesh(), FName("VestSocket"));

	helmetMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Helmet"));
	helmetMeshComp->SetupAttachment(GetMesh(), FName("HelmetSocket"));

	gunMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gun"));
	gunMeshComp->SetupAttachment(GetMesh(), FName("RightHandSocket"));

	firePoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FirePoint"));
	firePoint->SetupAttachment(gunMeshComp);
	firePoint->SetRelativeLocationAndRotation(FVector(10.0f, 0.0f, 30.0f), FRotator(90.0f, 0.0f, 180.0f));

	laserPoint = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LaserPointer"));
	laserPoint->SetupAttachment(firePoint);
	laserPoint->SetAutoActivate(false);

}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	currentHP = maxHP;

	FindPlayer();
	FindMusic();
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (enemyState)
	{
	case EEnemyState::IDLE:
		Idle(DeltaTime);
		break;
	case EEnemyState::MOVE:
		move(DeltaTime);
		break;
	case EEnemyState::AIM:
		Aim(DeltaTime);
		break;
	case EEnemyState::SHOOT:
		Shoot();
		break;
	case EEnemyState::DIE:
		Die();
		break;
	default:
		break;
	}

}

// Called to bind functionality to input
void ABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseEnemy::FindPlayer()
{
	TArray<AGunPlayer*> players;

	for(TActorIterator<AGunPlayer> iter(GetWorld()); iter; ++iter) {
		players.Add(*iter);
	}

	if(players.Num() > 0){
		playerREF = players[0];
	}
	else{
		playerREF = nullptr;
	}
}

void ABaseEnemy::FindMusic()
{
	TArray<AMusicActor*> musics;

	for (TActorIterator<AMusicActor> iter(GetWorld()); iter; ++iter) {
		musics.Add(*iter);
	}

	if (musics.Num() > 0) {
		musicREF = musics[0];
	}
	else {
		musicREF = nullptr;
	}
}

void ABaseEnemy::Idle(float deltaTime)
{
	currentTimer = FMath::Min(currentTimer + deltaTime, idleCooldown);
	if(currentTimer == idleCooldown){
		currentTimer = 0.0f;
		SetActorRotation((targetPlace - GetActorLocation()).Rotation());
		enemyState = EEnemyState::MOVE;
	}
}

void ABaseEnemy::move(float deltaTime)
{
	if(FVector::Distance(GetActorLocation(), targetPlace) < 34.0f) {
		enemyState = EEnemyState::AIM;
	}
	else {
		FVector toward = targetPlace - GetActorLocation();
		toward.Normalize();
		AddMovementInput(toward, speed);
	}
}

void ABaseEnemy::Aim(float deltaTime)
{
	if (numToFire > fireCounter) {
		currentTimer = FMath::Min(currentTimer + deltaTime, fireCooldown);
	}

	if (currentTimer == fireCooldown) {
		currentTimer = 0.0f;
		enemyState = EEnemyState::SHOOT;
	}
	else {
		//플레이어를 조준하는 명령어를 넣기
		if(playerREF != nullptr) {
			FVector aimDir = playerREF->VRCamera->GetComponentLocation() - GetActorLocation();
			if (numToFire > fireCounter) {
				laserPoint->SetWorldRotation(aimDir.Rotation());
				laserPoint->Activate(true);
			}
			aimDir.Z = 0.0f;
			SetActorRotation(aimDir.Rotation());
		}
	}
}

void ABaseEnemy::Shoot()
{
	if (playerREF != nullptr && numToFire > fireCounter) {
		laserPoint->Deactivate();
		FVector aimDir = playerREF->VRCamera->GetComponentLocation() - GetActorLocation();
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		GetWorld()->SpawnActor<ABulletActor>(bulletFactory, firePoint->GetComponentLocation(), aimDir.Rotation(), params);
		fireCounter++;
		FTimerHandle aimTimer;
		GetWorldTimerManager().SetTimer(aimTimer, FTimerDelegate::CreateLambda([&]() {
			enemyState = EEnemyState::AIM;
			}), 2.0f, false);
	}
}

void ABaseEnemy::Die()
{
	if (!bIsDead) {
		bIsDead = true;
		//타이머로 죽는 애니메이션 처리
	}
}

bool ABaseEnemy::Hit()
{
	UE_LOG(LogTemp, Warning, TEXT("%f"), musicREF->BeatAccuracy());
	//-0.5 의 절대값으로 들어오므로, 0.5가 가장 정확, 0에 가까울수록 부정확.
	if(enemyState != EEnemyState::DIE) {
		currentHP--;
		if(currentHP > 0){
			//맞는 이펙트 재생

			return false;
		}
		else {
			enemyState = EEnemyState::DIE;
			//죽는 이펙트 재생

			return true;
		}
	}
	else{
		return true;
	}
}

EEnemyState ABaseEnemy::GetEnemyState()
{
	return enemyState;
}

void ABaseEnemy::ABP_Death()
{
	Destroy();
}

