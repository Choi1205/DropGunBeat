// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include "Math/UnrealMathUtility.h"
#include "GunPlayer.h"
#include "musicGameInstance.h"


void UScoreWidget::NativeConstruct()
{
	UmusicGameInstance* GI = Cast <UmusicGameInstance>(GetGameInstance());

	// 파일에 있는 최고 점수 값을 읽어서 bestPoint 변수에 넣는다.
	FString loadPoint;
	FString fullPath = FPaths::ProjectContentDir() + FString("/Save/BestScore.txt");
	FFileHelper::LoadFileToString(loadPoint, *fullPath);
	GI->bestBBKKScore = FCString::Atoi(*loadPoint);

	SetScore();
	BBKKScore();
	SaveBestScore();
	
	GI->currentScore = 0;


	

	// bestPoint 변수의 값을 출력한다.
	BBKKBastScore->SetText(FText::FromString(FString::Printf(TEXT("%d"), GI->bestBBKKScore)));
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

	bestBBKKPoint = GI->currentScore;
	

	if (GI->bestBBKKScore < bestBBKKPoint)
	{
		GI->bestBBKKScore = bestBBKKPoint;
	}

	FString ScoreString = FString::Printf(TEXT("%d"), GI->bestBBKKScore);
	FText ScoreText = FText::FromString(ScoreString);
	BBKKBastScore->SetText(ScoreText);
}

void UScoreWidget::SaveBestScore()
{
	// 최고 점수를 파일(.txt)로 저장한다.
	UmusicGameInstance* GI = Cast <UmusicGameInstance>(GetGameInstance());
	FString savePoint = FString::Printf(TEXT("%d"), GI->bestBBKKScore);
	FString fullPath = FPaths::ProjectContentDir() + FString("/Save/BestScore.txt");

	FFileHelper::SaveStringToFile(savePoint, *fullPath);
}
