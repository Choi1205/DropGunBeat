// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyScoreWidget.generated.h"

/**
 * 
 */
UCLASS()
class DROPGUNBEAT_API UEnemyScoreWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, meta=(BindWidget))
	class UTextBlock* scoreBlock;
	UPROPERTY(BlueprintReadOnly, Transient, meta=(BindWidgetAnim))
	class UWidgetAnimation* scoreRise;


	FTimerHandle showingTimer;

	void ShowScore(int32 score, int32 multi);
};
