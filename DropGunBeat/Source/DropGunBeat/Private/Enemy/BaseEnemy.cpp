// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BaseEnemy.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "NiagaraComponent.h"
#include "EngineUtils.h"
#include "GunPlayer.h"
#include "BulletActor.h"
#include "Enemy/MusicActor.h"
#include "Enemy/EnemyScoreWidget.h"
#include "musicGameInstance.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetRootComponent()->SetWorldScale3D(FVector(1.5f));

	GetCapsuleComponent()->SetCollisionProfileName(FName("Enemy"));

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -88.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetCollisionProfileName(FName("NoCollision"));

	vestMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Vest"));
	vestMeshComp->SetupAttachment(GetMesh(), FName("VestSocket"));
	vestMeshComp->SetCollisionProfileName(FName("NoCollision"));

	helmetMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Helmet"));
	helmetMeshComp->SetupAttachment(GetMesh(), FName("HelmetSocket"));
	helmetMeshComp->SetCollisionProfileName(FName("NoCollision"));

	gunMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gun"));
	gunMeshComp->SetupAttachment(GetMesh(), FName("RightHandSocket"));
	gunMeshComp->SetCollisionProfileName(FName("NoCollision"));

	firePoint = CreateDefaultSubobject<USceneComponent>(TEXT("FirePoint"));
	firePoint->SetupAttachment(gunMeshComp);
	firePoint->SetRelativeLocationAndRotation(FVector(10.0f, 0.0f, 30.0f), FRotator(90.0f, 0.0f, 180.0f));

	laserPoint = CreateDefaultSubobject<UNiagaraComponent>(TEXT("LaserPointer"));
	laserPoint->SetupAttachment(firePoint);
	laserPoint->SetAutoActivate(false);

	scoreWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("ScoreWidget"));
	scoreWidgetComp->SetupAttachment(RootComponent);
	scoreWidgetComp->SetRelativeLocation(FVector(0.0f, 0.0f, 110.0f));
	scoreWidgetComp->SetWidgetSpace(EWidgetSpace::World);
	scoreWidgetComp->SetCollisionProfileName(FName("NoCollision"));

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	currentHP = maxHP;

	FindPlayer();
	FindMusic();

	idleCooldown *= beatTime;
	fireCooldown *= beatTime;

	gi = Cast<UmusicGameInstance>(GetGameInstance());
	scoreWidget = Cast<UEnemyScoreWidget>(scoreWidgetComp->GetWidget());
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
		beatTime = musicREF->GetBeatTime();
	}
	else {
		musicREF = nullptr;
		beatTime = 1.0f;
	}
}

void ABaseEnemy::Idle(float deltaTime)
{
	currentTimer = FMath::Min(currentTimer + deltaTime, idleCooldown);
	if(currentTimer == idleCooldown){
		currentTimer = 0.0f;
		FVector towardVec = targetPlace - GetActorLocation();
		towardVec.Z = 0.0f;
		SetActorRotation(towardVec.Rotation());
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
			aimDir = playerREF->boxcomp->GetComponentLocation() - GetActorLocation();
			aimDir.Z = 0.0f;
			SetActorRotation(aimDir.Rotation());
			aimDir = playerREF->boxcomp->GetComponentLocation() - firePoint->GetComponentLocation() + FVector(16.0f, 0.0f, -5.0f);
			if (numToFire > fireCounter) {
				laserPoint->SetWorldRotation(aimDir.Rotation());
				laserPoint->Activate(true);
			}
		}
	}
}

void ABaseEnemy::Shoot()
{
	if (!bIsFired && playerREF != nullptr && numToFire > fireCounter) {
		laserPoint->Deactivate();
		aimDir = playerREF->boxcomp->GetComponentLocation() - firePoint->GetComponentLocation() + FVector(16.0f, 0.0f, 0.0f);
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		GetWorld()->SpawnActor<ABulletActor>(bulletFactory, firePoint->GetComponentLocation(), aimDir.Rotation(), params);
		fireCounter++;
		bIsFired = true;
		FTimerHandle aimTimer;
		GetWorldTimerManager().SetTimer(aimTimer, FTimerDelegate::CreateLambda([&]() {
			enemyState = EEnemyState::AIM;
			bIsFired = false;
			}), 2.0f, false);
	}
}

bool ABaseEnemy::Hit(bool bIsPunch, int32 multiply)
{
	//UE_LOG(LogTemp, Warning, TEXT("%f"), musicREF->BeatAccuracy());
	//-0.5 의 절대값으로 들어오므로, 0.5가 가장 정확, 0에 가까울수록 부정확.
	FVector showDir = playerREF->boxcomp->GetComponentLocation() - GetActorLocation();
	showDir.Z = 0.0f;
	scoreWidgetComp->SetWorldRotation(showDir.Rotation());
	if (bIsPunch) {
		currentHP = 0;
		gi->currentScore += 600;
		scoreWidget->ShowScore(600);
	}
	else {
		float accuracy = 1.0f;
		if (musicREF) {
			accuracy = musicREF->BeatAccuracy();
		}

		if (enemyState != EEnemyState::DIE) {
			currentHP--;

			if (gi != nullptr && scoreWidget != nullptr) {
				if (accuracy < 0.2) {
					gi->currentScore += 400;
					scoreWidget->ShowScore(400);
				}
				else if (accuracy < 0.4) {
					gi->currentScore += 300;
					scoreWidget->ShowScore(300);
				}
				else {
					gi->currentScore += 200;
					scoreWidget->ShowScore(200);
				}
			}
		}
	}

	if (currentHP > 0) {
		return false;
	}
	else {
		enemyState = EEnemyState::DIE;
		GetCapsuleComponent()->SetCollisionProfileName(FName("NoCollision"));
		return true;
	}
}

EEnemyState ABaseEnemy::GetEnemyState()
{
	return enemyState;
}

void ABaseEnemy::SetTargetPlace(FVector value)
{
	targetPlace = value;
}

void ABaseEnemy::ABP_Death()
{
	Destroy();
}

