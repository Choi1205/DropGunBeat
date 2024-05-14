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

	UPROPERTY(EditAnywhere ,Category = "MySettings", meta = (BindWidget))
	class UTextBlock* BBKKBastScore;


	void SetScore();
	void BBKKScore();
	void SaveBestScore();

	UPROPERTY()
	class AGunPlayer* player;

	void NativeConstruct(); // 비긴플레이
	
private:
	int32 bestBBKKPoint = 0;

	int32 bestNightPoint = 0;

};
