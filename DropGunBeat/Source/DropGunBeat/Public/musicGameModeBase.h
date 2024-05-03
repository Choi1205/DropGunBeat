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
	void AddPoint(int32 count);
	

private:
	int32 Point = 0;
	
protected:
	
	
};
