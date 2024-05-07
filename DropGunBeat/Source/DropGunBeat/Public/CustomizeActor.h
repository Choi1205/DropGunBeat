// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CustomizeActor.generated.h"

UCLASS()
class DROPGUNBEAT_API ACustomizeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ACustomizeActor();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	class USceneComponent* sceneComp;

	UPROPERTY(EditAnywhere, Category = "Widget")
	class UCustomizeWidget* CustomWidget;

protected:
	virtual void BeginPlay() override;



};
