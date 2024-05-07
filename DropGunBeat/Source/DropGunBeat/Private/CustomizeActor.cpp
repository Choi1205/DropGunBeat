// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomizeActor.h"
#include "CustomizeWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/WidgetComponent.h>


ACustomizeActor::ACustomizeActor()
{
	PrimaryActorTick.bCanEverTick = true;

	sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	sceneComp->SetupAttachment(RootComponent);

	RobeCustomComponent = CreateDefaultSubobject <UWidgetComponent>(TEXT("Player Gun Component"));
	RobeCustomComponent->SetupAttachment(sceneComp);
}


void ACustomizeActor::BeginPlay()
{
	Super::BeginPlay();
	
	CustomWidget = Cast<UCustomizeWidget>(RobeCustomComponent->GetWidget());

}


void ACustomizeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

