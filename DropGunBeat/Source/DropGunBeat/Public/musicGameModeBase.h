// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "musicGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class DROPGUNBEAT_API AmusicGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "MySettings")
	TSubclassOf<class UPlayerWidget> PlayerWidget_BP;

	void AddPoint(int32 count);
	void remainBullet (int32 bulletCount);

private:
	int32 Point = 0;
	int32 bullet = 15;
	class UPlayerWidget* playerUI;

protected:
	virtual void BeginPlay() override;
	
};
