// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomizeActor.h"


ACustomizeActor::ACustomizeActor()
{
	PrimaryActorTick.bCanEverTick = true;

	sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	sceneComp->SetupAttachment(RootComponent);

}


void ACustomizeActor::BeginPlay()
{
	Super::BeginPlay();
	
}


void ACustomizeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

