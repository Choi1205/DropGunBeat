// Fill out your copyright notice in the Description page of Project Settings.


#include "GunPlayer.h"
#include "Math/UnrealMathUtility.h"
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/InputComponent.h>
#include <../../../../../../../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/PlayerController.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Camera/CameraComponent.h>
#include <../../../../../../../Source/Runtime/HeadMountedDisplay/Public/MotionControllerComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/SkeletalMeshComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "Enemy/BaseEnemy.h"
#include "BulletActor.h"
#include <../../../../../../../Source/Runtime/Core/Public/Delegates/Delegate.h>
#include <../../../../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraFunctionLibrary.h>


AGunPlayer::AGunPlayer()
{
 
	PrimaryActorTick.bCanEverTick = true;

	// VR ī�޶� ������Ʈ�� �����ϰ� ��Ʈ�� ���̰�ʹ�.
	VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));
	VRCamera->SetupAttachment(RootComponent);

	// �����Ʈ�ѷ� �޼�, ������ �����ϰ� ��Ʈ�� ���̰� �ʹ�.
	MotionLeft = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionLeft"));
	MotionLeft->SetTrackingMotionSource(TEXT("Left"));
	MotionLeft->SetupAttachment(RootComponent);

	MotionRight = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionRight"));
	MotionRight->SetTrackingMotionSource(TEXT("Right"));
	MotionRight->SetupAttachment(RootComponent);

	// �޼�, ������ ���̷�Ż�޽�������Ʈ�� ���� �����Ʈ�ѷ��� ���̰�ʹ�.
	MeshLeft = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshLeft"));
	MeshLeft->SetupAttachment(MotionLeft);
	MeshRight = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshRight"));
	MeshRight->SetupAttachment(MotionRight);

	// �޼�, ������ ���̷�Ż �޽ø��ε��ؼ� �����ϰ�ʹ�.
	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMeshLeft(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/SKM_MannyXR_left.SKM_MannyXR_left'"));
	// �ε� �����ߴٸ� �����ϰ�ʹ�.
	if (TempMeshLeft.Succeeded())
	{
		MeshLeft->SetSkeletalMesh(TempMeshLeft.Object);
		MeshLeft->SetWorldLocationAndRotation(FVector(-2.981260, -3.500000, 4.561753), FRotator(-25.000000f, -180.0f, 90.0f));
	}

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMeshRight(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/MannequinsXR/Meshes/SKM_MannyXR_right.SKM_MannyXR_right'"));
	// �ε� �����ߴٸ� �����ϰ�ʹ�.
	if (TempMeshRight.Succeeded())
	{
		MeshRight->SetSkeletalMesh(TempMeshRight.Object);
		MeshRight->SetWorldLocationAndRotation(FVector(-2.981260, 3.500000, 4.561753), FRotator(25.000000f, 0.000000f, 90.0f));
	}


	//boxComp->SetGenerateOverlapEvents(true);

}



void AGunPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	startLoc = GetActorLocation();
	targetLoc = GetActorLocation() + FVector(1000,0,0);

	

	// �÷��̾���Ʈ�ѷ��� ��������ʹ�.
	auto* pc = Cast<APlayerController>(Controller); // AVRPlayer�� ��Ʈ�ѷ��� �������°�

	// UEnhancedInputLocalPlayerSubsystem�� �����ͼ�
	if (pc != nullptr)
	{
		auto subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subsystem)
		{
			// AddMappingContext�� ȣ���ϰ�ʹ�.
			subsystem->AddMappingContext(IMC_GunPlayer, 0);
		}
		pc->SetShowMouseCursor(true);
	}

	

	MeshLeft->OnComponentHit.AddDynamic(this, &AGunPlayer::OnHit);

}


void AGunPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	moveTime += DeltaTime;

	

	if (moveTime < endTime)
	{
		//UE_LOG(LogTemp,Warning,TEXT("11111"));
		SetActorLocation(FMath::Lerp(startLoc, targetLoc, (moveTime/endTime)));
	}
	/*else
	{
		moveTime = 0;
		Score = UE_Log 

	}*/

	/*if (BaseEnemy->Perpect)
	{
		Score += 4;
		BaseEnemy->Perpect = false;
	}*/

	// �÷��̾� ���̾ư��� ��ġ
	/*playerfireLoc = GetActorLocation()+FVector(100,0,0); 
	playerfireRot = GetActorRotation();*/
}


void AGunPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);



	if (enhancedInputComponent != nullptr)
	{
		// �Լ��� ��ǲ ������Ʈ�� �����Ѵ�.
		enhancedInputComponent->BindAction(IA_Fire, ETriggerEvent::Started, this, &AGunPlayer::ONFire);
		enhancedInputComponent->BindAction(IA_Turn, ETriggerEvent::Triggered, this, &AGunPlayer::ONTurn);
	}

}

// �Ѿ� �߻� ��ǲ
void AGunPlayer::ONFire(const FInputActionValue& value)
{
	
	if (NI_Fire != nullptr)
	{
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NI_Fire, MeshRight->GetComponentLocation(),MeshRight->GetComponentRotation()); //���⿡�ٿ�����
	}

	UE_LOG(LogTemp, Warning, TEXT("2222222"));
	bool isPressed = value.Get<bool>();   // .�� ĳ��Ʈ�� �ǹ� , �����ڰ� ��� ������ ������ �� ��� �̷������� �ۼ�
	if (isPressed)
	{
		UE_LOG(LogTemp, Warning, TEXT("1111111"));
		//���콺 Ŀ�� �Է°� Ȯ��
		FVector WorldPosition, WorldDirection;
		APlayerController* MyController = Cast<APlayerController>(GetController());
		MyController->DeprojectMousePositionToWorld(WorldPosition, WorldDirection);

		FHitResult hitInfo;   // ���콺�� ��Ʈ�Ǿ�����

		if (GetWorld()->LineTraceSingleByChannel(hitInfo, WorldPosition, WorldPosition + WorldDirection * 10000, ECC_Visibility))
		{
			targetPos = hitInfo.ImpactPoint;    //��Ʈ�� ��ǥ
			
			//targetPos.Z = GetActorLocation().Z;   //z ��ǥ�� �÷��̾��� ��ǥ�� ����


			// ��ǥ ���� Ȯ��
			DrawDebugSphere(GetWorld(), hitInfo.ImpactPoint, 10.0f, 15, FColor::Red, false, 3, 1, 1);
			UE_LOG(LogTemp, Warning, TEXT("%.1f, %.1f, %.1f"), hitInfo.ImpactPoint.X, hitInfo.ImpactPoint.Y, hitInfo.ImpactPoint.Z);
		}
	}
}

void AGunPlayer::ONTurn(const FInputActionValue& value)
{
	float v = value.Get<float>();
	AddControllerYawInput(v);
}


//void AGunPlayer::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	// ���������� �ϱ�
//			
//			
//		
//
//}

void AGunPlayer::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	player = Cast<AGunPlayer>(OtherActor);
	
	if (OtherActor->IsA<ABaseEnemy>())
	{
		enemy->Hit();
	}
	else if (OtherActor->IsA<ABulletActor>())
	{
		if (bshield == true)
		{
			bshield = false;
		}
		else if (bshield == false)
		{
			Destroy();
		}

	}

}



	