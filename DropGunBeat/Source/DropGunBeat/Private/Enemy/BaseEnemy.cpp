// Fill out your copyright notice in the Description page of Project Settings.
// 기본이 되는 에너미. 리듬게임으로 치면 노트에 해당한다.
// 체력은 직접 입력해야 하므로 체력 1,2,4인 3종류의 블루프린트를 만들어서 사용.
// 조끼, 헬맷 컴포넌트에 스태틱매쉬를 집어넣어서 플레이어에게 체력이 다르다는 것을 표시
// 대기시간, 이동방향, 이동속도를 가지고 있으며, 언리얼 상에서 값을 조정해서 사용한다.

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
	//에너미는 대기->이동->(조준->발사) 순서로 행동하며, ()부분은 발사 횟수만큼 반복한다.
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

//언리얼에서 입력된 대기시간이 종료되면 이동을 실시한다.
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

//언리얼에서 입력한 이동 목표지점에 도달하면 조준을 시작한다.
void ABaseEnemy::move(float deltaTime)
{
	if(FVector::Distance(GetActorLocation(), targetPlace) < 34.0f) {
		//플레이어를 조준하는 레이저를 켠다
		laserPoint->Activate(true);
		enemyState = EEnemyState::AIM;
	}
	else {
		FVector toward = targetPlace - GetActorLocation();
		toward.Normalize();
		AddMovementInput(toward, speed);
	}
}

//조준중에는 플레이어에게 붉은색 레이저로 탄환이 날아오는 방향을 표시한다.
void ABaseEnemy::Aim(float deltaTime)
{
	if (numToFire > fireCounter) {
		currentTimer = FMath::Min(currentTimer + deltaTime, fireCooldown);
	}
	//조준 시간이 끝나면 발사한다.
	if (currentTimer == fireCooldown) {
		currentTimer = 0.0f;
		enemyState = EEnemyState::SHOOT;
	}
	else {
		//레이저 방향을 플레이어 카메라 쪽으로 변경한다.
		if(playerREF != nullptr) {
			aimDir = playerREF->boxcomp->GetComponentLocation() - GetActorLocation();
			aimDir.Z = 0.0f;
			SetActorRotation(aimDir.Rotation());
			//플레이어 판정의 중앙을 노리면 시야에 방해가 되므로, 앞쪽을 노린다.
			aimDir = playerREF->boxcomp->GetComponentLocation() - firePoint->GetComponentLocation() + FVector(16.0f, 0.0f, -5.0f);
			if (numToFire > fireCounter) {
				laserPoint->SetWorldRotation(aimDir.Rotation());
			}
		}
	}
}

//데드레커닝을 이용해서 예상 플레이어 위치를 구한 뒤 발사한다.
void ABaseEnemy::Shoot()
{
	if (!bIsFired && playerREF != nullptr && numToFire > fireCounter) {
		laserPoint->Deactivate();
		//플레이어와 발사구 사이의 거리를 구함
		float playerDist= (playerREF->GetActorLocation() - firePoint->GetComponentLocation()).Length();
		//거리를 탄환의 속도로 나누어 도달 시간을 계산. 탄환의 속도는 1000.0f
		float hitTime = playerDist / BULLETSPEED;
		//플레이어의 이동 속도에 도달시간을 곱하여 예상 위치를 구한다.
		FVector hitPoint = GetActorLocation() + GetActorForwardVector() * playerREF->moveDist;
		//예상위치와 발사구의 위치를 통해 발사할 백터방향을 구하고, 플레이어 판정 크기만큼 보정한다.
		aimDir = hitPoint - firePoint->GetComponentLocation() + FVector(16.0f, 0.0f, 0.0f);
		//탄환을 발사한다.
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

//플레이어 공격을 받았을때 정확도에 따라서 다른 점수를 위젯으로 출력한다.
bool ABaseEnemy::Hit(bool bIsPunch, int32 multiply)
{
	//UE_LOG(LogTemp, Warning, TEXT("%f"), musicREF->BeatAccuracy());
	//-0.5 의 절대값으로 들어오므로, 0.5가 가장 정확, 0에 가까울수록 부정확.
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
			//예상과 다르게 정확도가 반대로 되고 있었다. 최적화 문제가 아니었을까 추정 중.
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
	//플레이어 점수 배율처리를 위해 죽었는지, 안죽었는지 리턴한다.
	if (currentHP > 0) {
		return false;
	}
	else {
		enemyState = EEnemyState::DIE;
		GetCapsuleComponent()->SetCollisionProfileName(FName("NoCollision"));
		return true;
	}
}

//FSM용 상태 리턴 함수
EEnemyState ABaseEnemy::GetEnemyState()
{
	return enemyState;
}

//새로 생성되는 에너미의 이동 위치를 입력받기 위한 함수.
void ABaseEnemy::SetTargetPlace(FVector value)
{
	targetPlace = value;
}

void ABaseEnemy::ABP_Death()
{
	Destroy();
}

