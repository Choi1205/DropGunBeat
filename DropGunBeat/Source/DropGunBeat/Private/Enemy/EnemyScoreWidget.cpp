// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyScoreWidget.h"
#include "Components/TextBlock.h"

void UEnemyScoreWidget::ShowScore(int32 score)
{
	scoreBlock->SetText(FText::FromString(FString::FromInt(score)));

	if (score == 400) {
		scoreBlock->SetColorAndOpacity(FColor::Red);
	}
	else if (score == 300) {
		scoreBlock->SetColorAndOpacity(FColor::Yellow);
	}
	else {
		scoreBlock->SetColorAndOpacity(FColor::White);
	}

	scoreBlock->SetVisibility(ESlateVisibility::Visible);

	if (GetWorld()->GetTimerManager().IsTimerActive(showingTimer)) {
		GetWorld()->GetTimerManager().ClearTimer(showingTimer);
		StopAnimation(scoreRise);
	}
	PlayAnimation(scoreRise);

	GetWorld()->GetTimerManager().SetTimer(showingTimer, FTimerDelegate::CreateLambda([&]() {
		scoreBlock->SetVisibility(ESlateVisibility::Hidden);
		}), 3.0f, false);
}
