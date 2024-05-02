// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h>
#include "GunPlayer.generated.h"

UENUM(BlueprintType)
enum class EPlayerState : uint8 {
	IDLE,
	MOVE,
	DIE
};

UCLASS()
class DROPGUNBEAT_API AGunPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	AGunPlayer();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

		// VRī�޶� ������Ʈ�� �����ϰ� ��Ʈ�� ���̰�ʹ�.
	UPROPERTY(EditDefaultsOnly) // EditDefaultsOnly - �б����� 
	class UCameraComponent* VRCamera;  // class - ���漱�� ���

	// �����Ʈ�ѷ� �޼�, ������ �����ϰ� ��Ʈ�� ���̰� �ʹ�.
	 UPROPERTY(EditDefaultsOnly)
	 class UMotionControllerComponent* MotionLeft;

	  UPROPERTY(EditDefaultsOnly)
	 class UMotionControllerComponent* MotionRight;

	// �޼�, ������ ���̷�Ż �޽ø� ����� �����Ʈ�ѷ��� ���̰�ʹ�.
	UPROPERTY(EditDefaultsOnly)
	class USkeletalMeshComponent* MeshLeft;

	UPROPERTY(EditDefaultsOnly)
	class USkeletalMeshComponent* MeshRight;


	UPROPERTY(EditAnywhere, Category = Speed)
	float PlayerMovespeed = 0.5f;

	// ��ǲ
	UPROPERTY(EditDefaultsOnly, Category = VR)
	class UInputMappingContext* IMC_GunPlayer;

	UPROPERTY(EditDefaultsOnly, Category = VR)
	class UInputAction* IA_Fire;

	UPROPERTY(EditDefaultsOnly, Category = VR)
	class UInputAction* IA_Turn;

	void ONFire(const FInputActionValue& value);
	void ONTurn(const FInputActionValue& value);

	//void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// ������
		UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void OnDamaged();

	// ���̾ư���
	UPROPERTY(EditAnywhere)
	class UNiagaraSystem* NI_Fire;

	bool bshield = true;

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	float endTime = 3.f;
	UPROPERTY()
	float moveTime = 0.0f;

	UPROPERTY()
	int32 Score =0;

	UPROPERTY()
	FVector targetLoc;

	UPROPERTY()
	FVector startLoc;

	UPROPERTY()
	FVector targetPos;

	//UPROPERTY()
	//FVector playerfireLoc;

	//UPROPERTY()
	//FRotator playerfireRot;


	UPROPERTY(EditAnywhere, Category = "mysettings")
	TSubclassOf<UDamageType> DamageType;
	
	UPROPERTY()
	class AGunPlayer* player;

	UPROPERTY()
	class ABaseEnemy* enemy;

	UPROPERTY()
	class ABulletActor* bullet;

};
