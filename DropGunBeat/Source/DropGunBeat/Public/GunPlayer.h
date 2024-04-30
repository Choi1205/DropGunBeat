// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputActionValue.h>
#include "GunPlayer.generated.h"


UCLASS()
class DROPGUNBEAT_API AGunPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	AGunPlayer();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

		// VR카메라 컴포넌트를 생성하고 루트에 붙이고싶다.
	UPROPERTY(EditDefaultsOnly) // EditDefaultsOnly - 읽기전용 
	class UCameraComponent* VRCamera;  // class - 전방선언 약식

	// 모션컨트롤러 왼손, 오른손 생성하고 루트에 붙이고 싶다.
	 UPROPERTY(EditDefaultsOnly)
	 class UMotionControllerComponent* MotionLeft;

	  UPROPERTY(EditDefaultsOnly)
	 class UMotionControllerComponent* MotionRight;

	// 왼손, 오른손 스켈레탈 메시를 만들고 모션컨트롤러에 붙이고싶다.
	UPROPERTY(EditDefaultsOnly)
	class USkeletalMeshComponent* MeshLeft;

	UPROPERTY(EditDefaultsOnly)
	class USkeletalMeshComponent* MeshRight;


	UPROPERTY(EditAnywhere, Category = Speed)
	float PlayerMovespeed = 0.5f;

	UPROPERTY(EditDefaultsOnly, Category = VR)
	class UInputMappingContext* IMC_GunPlayer;

	UPROPERTY(EditDefaultsOnly, Category = VR)
	class UInputAction* IA_Fire;


	void ONFire(const FInputActionValue& value);


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
	
};
