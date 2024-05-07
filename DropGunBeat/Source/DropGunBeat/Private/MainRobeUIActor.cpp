// Fill out your copyright notice in the Description page of Project Settings.


#include "MainRobeUIActor.h"

// Sets default values
AMainRobeUIActor::AMainRobeUIActor()
{
	PrimaryActorTick.bCanEverTick = true;

	sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	sceneComp->SetupAttachment(RootComponent);
}


void AMainRobeUIActor::BeginPlay()
{
	Super::BeginPlay();
	
}


void AMainRobeUIActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

