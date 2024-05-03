// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerWidget.generated.h"

/**
 * 
 */
UCLASS()
class DROPGUNBEAT_API UPlayerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	
	void SetCurrentBullet();

	UPROPERTY(EditAnywhere ,meta = ( BindWidget ))
	class UTextBlock* CurrentBullet;
	
	class AGunPlayer* player;



};
