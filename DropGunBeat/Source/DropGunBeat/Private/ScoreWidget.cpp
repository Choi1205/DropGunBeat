// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include "Math/UnrealMathUtility.h"
#include "GunPlayer.h"
#include "musicGameInstance.h"


void UScoreWidget::NativeConstruct()
{
	SetScore();

	UmusicGameInstance* GI = Cast <UmusicGameInstance>(GetGameInstance());
	GI->currentScore = 0;

	GI->bestBBKKScore = bestBBKKPoint;
}

void UScoreWidget::SetScore()
{
	UmusicGameInstance* GI = Cast <UmusicGameInstance>(GetGameInstance());
	
	FString ScoreString = FString::Printf(TEXT("%d"), GI->currentScore);
	FText ScoreText = FText::FromString(ScoreString);
	CurrentScore->SetText(ScoreText);
}

void UScoreWidget::BBKKScore()
{
	UmusicGameInstance* GI = Cast <UmusicGameInstance>(GetGameInstance());

	FString ScoreString = FString::Printf(TEXT("%d"), GI->bestBBKKScore);
	FText ScoreText = FText::FromString(ScoreString);
	BBKKBastScore->SetText(ScoreText);

}
