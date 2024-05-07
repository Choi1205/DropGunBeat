// Fill out your copyright notice in the Description page of Project Settings.


#include "MainRobeUIActor.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/WidgetComponent.h>

// Sets default values
AMainRobeUIActor::AMainRobeUIActor()
{
	PrimaryActorTick.bCanEverTick = true;

	sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	sceneComp->SetupAttachment(RootComponent);

	
	MainUIComponent = CreateDefaultSubobject <UWidgetComponent>(TEXT("Player Gun Component"));
	MainUIComponent->SetupAttachment(sceneComp);
}


void AMainRobeUIActor::BeginPlay()
{
	Super::BeginPlay();
	
	MainRobeWidget = Cast<UMainRobeUserWidget>(MainUIComponent->GetWidget());

}


void AMainRobeUIActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

