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

	UPROPERTY(VisibleAnywhere, Category = "MySettings", meta = (BindWidget))
	class UImage* shieldPS;

	UPROPERTY(VisibleAnywhere, Category = "MySettings", meta = (BindWidget))
	class UImage* bshieldPS;

	UPROPERTY(VisibleAnywhere, Category = "MySettings", meta = (BindWidget))
	class UTextBlock* CurrentShield;

	void removeShield(int32 ShieldCount);

	void startShield();

	void hitShield();

	void produceShield();

private:
	int32 reShield = 4;

};
