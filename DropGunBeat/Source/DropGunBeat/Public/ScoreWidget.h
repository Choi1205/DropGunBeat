// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreWidget.generated.h"

/**
 * 
 */
UCLASS()
class DROPGUNBEAT_API UScoreWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere ,Category = "MySettings", meta = (BindWidget))
	class UTextBlock* CurrentScore;

	void SetScore();

	UPROPERTY()
	class AGunPlayer* player;

	void NativeConstruct(); // 비긴플레이
	
private:
	int32 bestBBKKScore = 0;

	int32 bestNightScore = 0;

};
