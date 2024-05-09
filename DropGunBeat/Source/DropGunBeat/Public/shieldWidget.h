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

	void startShield();

};
