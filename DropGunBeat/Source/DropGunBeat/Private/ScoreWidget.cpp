// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>
#include "Math/UnrealMathUtility.h"
#include "GunPlayer.h"
#include "musicGameInstance.h"


void UScoreWidget::NativeConstruct()
{
	UmusicGameInstance* GI = Cast <UmusicGameInstance>(GetGameInstance());

	// ���Ͽ� �ִ� �ְ� ���� ���� �о bestPoint ������ �ִ´�.
	FString loadPoint;
	FString fullPath = FPaths::ProjectContentDir() + FString("/Save/BestScore.txt");
	FFileHelper::LoadFileToString(loadPoint, *fullPath);
	GI->bestBBKKScore = FCString::Atoi(*loadPoint);

	SetScore();
	BBKKScore();
	SaveBestScore();
	
	GI->currentScore = 0;


	

	// bestPoint ������ ���� ����Ѵ�.
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
	// �ְ� ������ ����(.txt)�� �����Ѵ�.
	UmusicGameInstance* GI = Cast <UmusicGameInstance>(GetGameInstance());
	FString savePoint = FString::Printf(TEXT("%d"), GI->bestBBKKScore);
	FString fullPath = FPaths::ProjectContentDir() + FString("/Save/BestScore.txt");

	FFileHelper::SaveStringToFile(savePoint, *fullPath);
}
