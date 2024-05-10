// Fill out your copyright notice in the Description page of Project Settings.


#include "shieldWidget.h"
#include "Components/Image.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>


void UshieldWidget::startShield()
{

	shieldPS->SetVisibility(ESlateVisibility::Visible);
	bshieldPS->SetVisibility(ESlateVisibility::Hidden);
	CurrentShield->SetVisibility(ESlateVisibility::Hidden);

}

void UshieldWidget::hitShield()
{
	shieldPS->SetVisibility(ESlateVisibility::Hidden);
	bshieldPS->SetVisibility(ESlateVisibility::Visible);
	CurrentShield->SetVisibility(ESlateVisibility::Visible);
	CurrentShield->SetText(FText::FromString("4"));

}

void UshieldWidget::produceShield()
{
	shieldPS->SetVisibility(ESlateVisibility::Visible);
	bshieldPS->SetVisibility(ESlateVisibility::Hidden);
	CurrentShield->SetVisibility(ESlateVisibility::Hidden);
}

void UshieldWidget::removeShield(int32 ShieldCount)
{
	// 현재 점수에서 bulletCount만큼을 빼기
	reShield = FMath::Clamp(reShield + ShieldCount, 0, 4);

	FString ShieldString = FString::Printf(TEXT(" % d"), reShield);
	FText ShieldText = FText::FromString(ShieldString);
	//UE_LOG(LogTemp, Warning, TEXT("remain"));
	CurrentShield->SetText(ShieldText);
}
