// Fill out your copyright notice in the Description page of Project Settings.
// �⺻�� �Ǵ� ���ʹ�. ����������� ġ�� ��Ʈ�� �ش��Ѵ�.
// ü���� ���� �Է��ؾ� �ϹǷ� ü�� 1,2,4�� 3������ �������Ʈ�� ���� ���.
// ����, ��� ������Ʈ�� ����ƽ�Ž��� ����־ �÷��̾�� ü���� �ٸ��ٴ� ���� ǥ��
// ���ð�, �̵�����, �̵��ӵ��� ������ ������, �𸮾� �󿡼� ���� �����ؼ� ����Ѵ�.

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

#define BULLETSPEED 1000.0f

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
	//���ʹ̴� ���->�̵�->(����->�߻�) ������ �ൿ�ϸ�, ()�κ��� �߻� Ƚ����ŭ �ݺ��Ѵ�.
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

void ABaseEnemy::FindPlayer()
{
	for(TActorIterator<AGunPlayer> iter(GetWorld()); iter; ++iter) {
		playerREF = *iter;
	}
}

void ABaseEnemy::FindMusic()
{
	for (TActorIterator<AMusicActor> iter(GetWorld()); iter; ++iter) {
		musicREF = *iter;
	}

	if (musicREF) {
		beatTime = musicREF->GetBeatTime();
	}
	else {
		beatTime = 1.0f;
	}
}

//�𸮾󿡼� �Էµ� ���ð��� ����Ǹ� �̵��� �ǽ��Ѵ�.
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

//�𸮾󿡼� �Է��� �̵� ��ǥ������ �����ϸ� ������ �����Ѵ�.
void ABaseEnemy::move(float deltaTime)
{
	if(FVector::Distance(GetActorLocation(), targetPlace) < 34.0f) {
		//�÷��̾ �����ϴ� �������� �Ҵ�
		laserPoint->Activate(true);
		enemyState = EEnemyState::AIM;
	}
	else {
		FVector toward = targetPlace - GetActorLocation();
		toward.Normalize();
		AddMovementInput(toward, speed);
	}
}

//�����߿��� �÷��̾�� ������ �������� źȯ�� ���ƿ��� ������ ǥ���Ѵ�.
void ABaseEnemy::Aim(float deltaTime)
{
	if (numToFire > fireCounter) {
		currentTimer = FMath::Min(currentTimer + deltaTime, fireCooldown);
	}
	//���� �ð��� ������ �߻��Ѵ�.
	if (currentTimer == fireCooldown) {
		currentTimer = 0.0f;
		enemyState = EEnemyState::SHOOT;
	}
	else {
		//������ ������ �÷��̾� ī�޶� ������ �����Ѵ�.
		if(playerREF != nullptr) {
			aimDir = playerREF->boxcomp->GetComponentLocation() - GetActorLocation();
			aimDir.Z = 0.0f;
			SetActorRotation(aimDir.Rotation());
			//�÷��̾� ������ �߾��� �븮�� �þ߿� ���ذ� �ǹǷ�, ������ �븰��.
			aimDir = playerREF->boxcomp->GetComponentLocation() - firePoint->GetComponentLocation() + FVector(16.0f, 0.0f, -5.0f);
			if (numToFire > fireCounter) {
				laserPoint->SetWorldRotation(aimDir.Rotation());
			}
		}
	}
}

//���巹Ŀ���� �̿��ؼ� ���� �÷��̾� ��ġ�� ���� �� �߻��Ѵ�.
void ABaseEnemy::Shoot()
{
	if (!bIsFired && playerREF != nullptr && numToFire > fireCounter) {
		laserPoint->Deactivate();
		//�÷��̾�� �߻籸 ������ �Ÿ��� ����
		float playerDist= (playerREF->GetActorLocation() - firePoint->GetComponentLocation()).Length();
		//�Ÿ��� źȯ�� �ӵ��� ������ ���� �ð��� ���. źȯ�� �ӵ��� 1000.0f
		float hitTime = playerDist / BULLETSPEED;
		//�÷��̾��� �̵� �ӵ��� ���޽ð��� ���Ͽ� ���� ��ġ�� ���Ѵ�.
		FVector hitPoint = GetActorLocation() + GetActorForwardVector() * playerREF->moveDist;
		//������ġ�� �߻籸�� ��ġ�� ���� �߻��� ���͹����� ���ϰ�, �÷��̾� ���� ũ�⸸ŭ �����Ѵ�.
		aimDir = hitPoint - firePoint->GetComponentLocation() + FVector(16.0f, 0.0f, 0.0f);
		//źȯ�� �߻��Ѵ�.
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		GetWorld()->SpawnActor<ABulletActor>(bulletFactory, firePoint->GetComponentLocation(), aimDir.Rotation(), params);

		fireCounter++;
		bIsFired = true;
		FTimerHandle aimTimer;
		GetWorldTimerManager().SetTimer(aimTimer, FTimerDelegate::CreateLambda([&]() {
			laserPoint->Activate(true);
			enemyState = EEnemyState::AIM;
			bIsFired = false;
			}), 2.0f, false);
	}
}

//�÷��̾� ������ �޾����� ��Ȯ���� ���� �ٸ� ������ �������� ����Ѵ�.
bool ABaseEnemy::Hit(bool bIsPunch, int32 multiply)
{
	//UE_LOG(LogTemp, Warning, TEXT("%f"), musicREF->BeatAccuracy());
	//-0.5 �� ���밪���� �����Ƿ�, 0.5�� ���� ��Ȯ, 0�� �������� ����Ȯ.
	FVector showDir = playerREF->boxcomp->GetComponentLocation() - GetActorLocation();
	showDir.Z = 0.0f;
	scoreWidgetComp->SetWorldRotation(showDir.Rotation());
	if (bIsPunch) {
		currentHP = 0;
		gi->currentScore += 600 * multiply;
		scoreWidget->ShowScore(600, multiply);
	}
	else {
		float accuracy = 1.0f;
		if (musicREF) {
			accuracy = musicREF->BeatAccuracy();
		}

		if (enemyState != EEnemyState::DIE) {
			currentHP--;
			//����� �ٸ��� ��Ȯ���� �ݴ�� �ǰ� �־���. ����ȭ ������ �ƴϾ����� ���� ��.
			if (gi != nullptr && scoreWidget != nullptr) {
				if (accuracy < 0.2) {
					gi->currentScore += 400 * multiply;
					scoreWidget->ShowScore(400, multiply);
				}
				else if (accuracy < 0.4) {
					gi->currentScore += 300 * multiply;
					scoreWidget->ShowScore(300, multiply);
				}
				else {
					gi->currentScore += 200 * multiply;
					scoreWidget->ShowScore(200, multiply);
				}
			}
		}
	}
	//�÷��̾� ���� ����ó���� ���� �׾�����, ���׾����� �����Ѵ�.
	if (currentHP > 0) {
		return false;
	}
	else {
		enemyState = EEnemyState::DIE;
		GetCapsuleComponent()->SetCollisionProfileName(FName("NoCollision"));
		return true;
	}
}

//FSM�� ���� ���� �Լ�
EEnemyState ABaseEnemy::GetEnemyState()
{
	return enemyState;
}

//���� �����Ǵ� ���ʹ��� �̵� ��ġ�� �Է¹ޱ� ���� �Լ�.
void ABaseEnemy::SetTargetPlace(FVector value)
{
	targetPlace = value;
}

void ABaseEnemy::ABP_Death()
{
	Destroy();
}

