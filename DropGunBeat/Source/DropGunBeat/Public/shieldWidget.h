// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "shieldWidget.generated.h"

/**
 * 
 */
UCLASS()
class DROPGUNBEAT_API UshieldWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY()
	class AGunPlayer* player;

	// 쉴드
	UPROPERTY(VisibleAnywhere, Category = "MySettings", meta = (BindWidget))
	class UImage* shieldPS;

	// 쉴드파괴
	UPROPERTY(VisibleAnywhere, Category = "MySettings", meta = (BindWidget))
	class UImage* bshieldPS;

	// 점수 배경
	UPROPERTY(VisibleAnywhere, Category = "MySettings", meta = (BindWidget))
	class UImage* NumberRad;

	// X배경
	UPROPERTY(VisibleAnywhere, Category = "MySettings", meta = (BindWidget))
	class UImage* XRad;

	// 쉴드깨졌을때 숫자
	UPROPERTY(VisibleAnywhere, Category = "MySettings", meta = (BindWidget))
	class UTextBlock* CurrentShield;

	// 배율
	UPROPERTY(VisibleAnywhere, Category = "MySettings", meta = (BindWidget))
	class UTextBlock* NumberX;

	// X 텍스트
	UPROPERTY(VisibleAnywhere, Category = "MySettings", meta = (BindWidget))
	class UTextBlock* X;

	



	void removeShield(int32 ShieldCount);

	void startShield();

	void hitShield();

	void produceShield();

	void CurrentX(int32 _CurrentX);

private:
	int32 reShield = 4;

	int32 reX = 4;

};
