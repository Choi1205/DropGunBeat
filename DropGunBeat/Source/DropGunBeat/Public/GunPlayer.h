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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) // EditDefaultsOnly - �б����� 
	class USceneComponent* rollComp;

		// VRī�޶� ������Ʈ�� �����ϰ� ��Ʈ�� ���̰�ʹ�.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) // EditDefaultsOnly - �б����� 
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

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* leftScene;

	UPROPERTY(EditDefaultsOnly)
	class USceneComponent* rightScene;

	UPROPERTY(EditDefaultsOnly)
	class USphereComponent* LeftHitComp;

	UPROPERTY(EditDefaultsOnly)
	class USphereComponent* RightHitComp;

	UPROPERTY(EditDefaultsOnly)
	class UBoxComponent* boxcomp;

	UPROPERTY(EditAnywhere, Category = Speed)
	float PlayerMovespeed = 0.5f;

	// ����
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MySettings") // ����
	class USoundBase* fireSound;


	// ��ǲ
	UPROPERTY(EditDefaultsOnly, Category = VR)
	class UInputMappingContext* IMC_GunPlayer;

	UPROPERTY(EditDefaultsOnly, Category = VR)
	class UInputAction* IA_Fire;

	UPROPERTY(EditDefaultsOnly, Category = VR)
	class UInputAction* IA_LeftFire;

	/*UPROPERTY(EditDefaultsOnly, Category = VR)
	class UInputAction* IA_Reroad;*/

	UPROPERTY(EditDefaultsOnly, Category = VR)
	class UInputAction* IA_Turn;

	UPROPERTY(VisibleAnywhere, Category="MySettings|Components")
	class UWidgetComponent* PlayerGunWidgetComp;

	UPROPERTY(VisibleAnywhere, Category="MySettings|Components")
	class UWidgetComponent* PlayerGunWidgetLeftComp;

	UPROPERTY(EditAnywhere, Category = "Widget")
	class UPlayerWidget* PlayerWidget;
	
	UPROPERTY(EditAnywhere, Category = "Widget")
	class UPlayerWidget* PlayerLeftWidget;

	UPROPERTY(VisibleAnywhere, Category="MySettings|Components")
	class UWidgetComponent* PlayerShieldWidgetComp;

	void ONLeftFire(const FInputActionValue& value);
	void ONFire(const FInputActionValue& value);

	void ONTurn(const FInputActionValue& value);
	void ONReroad();
	void ONLeftReroad();
	//void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// ������
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnDamaged();

	UFUNCTION()
	void shieldrecovery();

	// ���̾ư���
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UNiagaraSystem* NI_Fire;

	bool bshield = true; // ����
	bool bGunbullet = true; // �Ѿ˽�� �޵���

	UPROPERTY(BlueprintReadWrite)
	bool bStart = false;

	// ���� Ȱ��ȭ
	bool bLeft;

	//ĳ���� �Ϸ� �Ǵܿ� ����
	UPROPERTY()
	bool bIsCastingDone = false;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	//class UParticleSystem* FX_FireHit;
	//��� ���� ����Ʈ�� ���ͷ� ����.
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ABulletFXActor> bulletFX;

	TArray<ABulletFXActor*> FXarray;

	int32 FXAddress = 0;

	void PlayFX(FVector playLoc);
	
	// źâ
	UPROPERTY()
	int32 bulletFactory = 0;


	UPROPERTY()
	int32 leftbulletFactory = 0;
	
	UFUNCTION(BlueprintCallable)
	void SetStartLoc();

	class UmusicGameInstance* gi = nullptr;
	
	// ����
	UPROPERTY()
	int32 CurrentXNumber = 0;

	int32 CurrentXEnemy = 1;

	void SetCurrentXNumber();

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	class APlayerController* pc;

	UPROPERTY()
	float endTime = 3.f;
	UPROPERTY()
	float moveTime = 0.0f;

	UPROPERTY()
	int32 Score =0;

	// �������
	UPROPERTY()
	int32 recovery = 0;

	UPROPERTY()
	FVector targetLoc;

	UPROPERTY()
	FVector startLoc;

	UPROPERTY()
	FVector targetPos;

	UPROPERTY(EditAnywhere, Category = "mysettings")
	TSubclassOf<UDamageType> DamageType;
	
	UPROPERTY()
	class AGunPlayer* player;

	UPROPERTY()
	class ABaseEnemy* enemy;

	UPROPERTY()
	class ABulletActor* bullet;

	UPROPERTY()
	class AMainRobeUIActor* widgetLevel;

	UPROPERTY(EditAnywhere, Category = "Widget")
	class UshieldWidget* shieldWidget;

	UPROPERTY(EditAnywhere, Category = "Widget")
	class UbShieldWidget* bshieldWidget;

	UPROPERTY()
	class AgameStartActor* startActor;

	UPROPERTY()
	class ACustomizeActor* WeaponActor;

	UPROPERTY()
	class AMusicActor* musicActor;

	void LeftGunToggle(bool value);

private:
	// ������ �÷��̾ �ٶ󺸰� 
	//FRotator BillboardWidgetComponent(class AActor* camActor);
};
