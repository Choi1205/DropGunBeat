// Fill out your copyright notice in the Description page of Project Settings.
// 에너미 액터의 에님 인스턴스. FSM을 사용하고 있다

#include "Enemy/EnemyAnimInstance.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	enemyREF = Cast<ABaseEnemy>(GetOwningActor());
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (enemyREF != nullptr) {
		enemyState = enemyREF->GetEnemyState();
	}
}

//사망 애니메이션 끝부분에서 액터 제거
void UEnemyAnimInstance::AnimNotify_Death()
{
	enemyREF->ABP_Death();
}
