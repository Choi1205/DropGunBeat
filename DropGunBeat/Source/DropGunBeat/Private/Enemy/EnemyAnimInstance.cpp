// Fill out your copyright notice in the Description page of Project Settings.
// ���ʹ� ������ ���� �ν��Ͻ�. FSM�� ����ϰ� �ִ�

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

//��� �ִϸ��̼� ���κп��� ���� ����
void UEnemyAnimInstance::AnimNotify_Death()
{
	enemyREF->ABP_Death();
}
