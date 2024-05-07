// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseEnemy.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8 {
	IDLE,
	MOVE,
	AIM,
	SHOOT,
	DIE
};

UENUM(BlueprintType)
enum class EAccuracy : uint8 {
	PERPECT,
	GOOD,
	BAD
};

UCLASS()
class DROPGUNBEAT_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseEnemy();

	//////////////
	// 컴포넌트 //
	//////////////
	
	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* vestMeshComp;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* helmetMeshComp;

	UPROPERTY(EditDefaultsOnly)
	class UStaticMeshComponent* gunMeshComp;

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* firePoint;

	UPROPERTY(EditDefaultsOnly)
	class UNiagaraComponent* laserPoint;

	UPROPERTY(EditDefaultsOnly)
	class UWidgetComponent* scoreWidgetComp;

private:

	//////////
	// 함수 //
	//////////

	void FindPlayer();
	void FindMusic();

	//////////
	// 변수 //
	//////////

	UPROPERTY(VisibleAnywhere, Category = "EnemyStat")
	EEnemyState enemyState;
	UPROPERTY(EditDefaultsOnly, Category = "EnemyStat|HP")
	int32 maxHP = 1;
	UPROPERTY(VisibleAnywhere, Category = "EnemyStat|HP")
	int32 currentHP = 1;
	UPROPERTY(EditAnywhere, Category = "EnemyStat|Attack")
	int32 numToFire;
	UPROPERTY(VisibleAnywhere, Category = "EnemyStat|Attack")
	int32 fireCounter;
	UPROPERTY(EditAnywhere, Category = "EnemyStat|Attack")
	float fireCooldown = 3.0f;
	UPROPERTY(EditDefaultsOnly, Category = "EnemyStat|Attack")
	TSubclassOf<class ABulletActor> bulletFactory;
	UPROPERTY(EditAnywhere, Category = "EnemyStat")
	float idleCooldown = 1.0f;
	UPROPERTY(VisibleAnywhere, Category = "EnemyStat")
	float currentTimer = 0.0f;
	UPROPERTY(EditAnywhere, Category = "EnemyStat|Move")
	FVector targetPlace;
	UPROPERTY(EditAnywhere, Category = "EnemyStat|Move")
	float speed = 1.0f;
	UPROPERTY(VisibleAnywhere, Category = "EnemyStat")
	class UEnemyScoreWidget* scoreWidget;

	class AGunPlayer* playerREF;

	class AMusicActor* musicREF;

	class UmusicGameInstance* gi;

	UPROPERTY(VisibleAnywhere)
	float beatTime;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//////////
	// 함수 //
	//////////
	
	void Idle(float deltaTime);
	void move(float deltaTime);
	void Aim(float deltaTime);
	void Shoot();
	//void Die();

	bool Hit(bool bIsPunch);

	EEnemyState GetEnemyState();

	void SetTargetPlace(FVector value);

	void ABP_Death();
};
