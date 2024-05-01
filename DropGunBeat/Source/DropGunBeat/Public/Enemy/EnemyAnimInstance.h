// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseEnemy.h"
#include "EnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class DROPGUNBEAT_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class ABaseEnemy* enemyREF;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EEnemyState enemyState;

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION()
	void AnimNotify_ShootTiming();
	UFUNCTION()
	void AnimNotify_Death();
};
