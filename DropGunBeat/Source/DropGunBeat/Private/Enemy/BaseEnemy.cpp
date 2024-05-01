// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/BaseEnemy.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EngineUtils.h"

// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -88.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, 0.0f, -90.0f));

	vestMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Vest"));
	vestMeshComp->SetupAttachment(GetMesh());

	helmetMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Helmet"));
	helmetMeshComp->SetupAttachment(GetMesh());

	gunMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gun"));
	gunMeshComp->SetupAttachment(GetMesh());

	firePoint = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FirePoint"));
	firePoint->SetupAttachment(gunMeshComp);

}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	currentHP = maxHP;

}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (enemyState)
	{
	case EPhaseState::IDLE:
		Idle(DeltaTime);
		break;
	case EPhaseState::MOVE:
		move(DeltaTime);
		break;
	case EPhaseState::AIM:
		Aim(DeltaTime);
		break;
	case EPhaseState::SHOOT:
		Shoot();
		break;
	case EPhaseState::DIE:
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

void ABaseEnemy::Idle(float deltaTime)
{
	currentTimer = FMath::Min(currentTimer + deltaTime, idleCooldown);
	if(currentTimer == idleCooldown){
		currentTimer = 0.0f;
		SetActorRotation((targetPlace - GetActorLocation()).Rotation());
		enemyState = EPhaseState::MOVE;
	}
}

void ABaseEnemy::move(float deltaTime)
{
	if(FVector::Distance(GetActorLocation(), targetPlace) < 34.0f) {
		enemyState = EPhaseState::AIM;
	}
	else {
		FVector toward = targetPlace - GetActorLocation();
		toward.Normalize();
		AddMovementInput(toward, speed);
	}
}

void ABaseEnemy::Aim(float deltaTime)
{
	currentTimer = FMath::Min(currentTimer + deltaTime, fireCooldown);
	if (currentTimer == fireCooldown) {
		currentTimer = 0.0f;
		enemyState = EPhaseState::SHOOT;
	}
	else {
		//플레이어를 조준하는 명령어를 넣기
	}
}

void ABaseEnemy::Shoot()
{
	
}

void ABaseEnemy::Die()
{
}

