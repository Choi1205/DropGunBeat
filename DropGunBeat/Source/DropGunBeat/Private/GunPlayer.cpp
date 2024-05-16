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
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/BoxComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Components/SphereComponent.h>
#include "PlayerWidget.h"
#include "musicGameModeBase.h"
#include <../../../../../../../Source/Runtime/Engine/Classes/GameFramework/GameModeBase.h>
#include <../../../../../../../Source/Runtime/UMG/Public/Components/WidgetComponent.h>
#include <../../../../../../../Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h>
#include <../../../../../../../Source/Runtime/Engine/Public/KismetTraceUtils.h>
#include "MainRobeUIActor.h"
#include "gameStartActor.h"
#include "bShieldWidget.h"
#include "shieldWidget.h"
#include "Enemy/MusicActor.h"
#include <../../../../../../../Source/Runtime/Engine/Public/EngineUtils.h>
#include "musicGameInstance.h"



AGunPlayer::AGunPlayer()
{
 
	PrimaryActorTick.bCanEverTick = true;

	// VR ī�޶� ������Ʈ�� �����ϰ� ��Ʈ�� ���̰�ʹ�.
	VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));
	VRCamera->SetupAttachment(RootComponent);

	// �ڽ� ������Ʈ�� ī�޶� �ٿ��� ���� ��ų� �Ѿ˿� ������ �������� �޴´�.
	boxcomp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	boxcomp->SetupAttachment(VRCamera);

	// �����Ʈ�ѷ� �޼�, ������ �����ϰ� ��Ʈ�� ���̰� �ʹ�.
	//MotionLeft = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionLeft"));
	//MotionLeft->SetTrackingMotionSource(TEXT("Left"));
	//MotionLeft->SetupAttachment(RootComponent);

	MotionRight = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionRight"));
	MotionRight->SetTrackingMotionSource(TEXT("Right"));
	MotionRight->SetupAttachment(RootComponent);

	// �޼�, ������ ���̷�Ż�޽�������Ʈ�� ���� �����Ʈ�ѷ��� ���̰�ʹ�.
	//MeshLeft = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshLeft"));
	//MeshLeft->SetupAttachment(MotionLeft);
	MeshRight = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshRight"));
	MeshRight->SetupAttachment(MotionRight);

	// �޼�, ������ ���̷�Ż �޽ø��ε��ؼ� �����ϰ�ʹ�.
	//ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMeshLeft(TEXT("/Script/Engine.SkeletalMesh'/Game/assets/pistol-desert-eagle-weapon-model-cs2/DEAGLE.DEAGLE'"));
	//// �ε� �����ߴٸ� �����ϰ�ʹ�.
	//if (TempMeshLeft.Succeeded())
	//{
	//	MeshLeft->SetSkeletalMesh(TempMeshLeft.Object);
	//	MeshLeft->SetWorldLocationAndRotation(FVector((6.7f,-11.25f,1.97f)), FRotator((75.0f,  0.0f,  180.0f)));
	//	MeshLeft->SetRelativeScale3D(FVector(20.0f));
	//}

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMeshRight(TEXT("/Script/Engine.SkeletalMesh'/Game/assets/pistol-desert-eagle-weapon-model-cs2/DEAGLE.DEAGLE'"));
	// �ε� �����ߴٸ� �����ϰ�ʹ�.
	if (TempMeshRight.Succeeded())
	{
		MeshRight->SetSkeletalMesh(TempMeshRight.Object);
		MeshRight->SetWorldLocationAndRotation(FVector((13.3f, 10.7f, -25.1f)), FRotator((35.0f, 0.0f, 180.0f)));
		MeshRight->SetRelativeScale3D(FVector(20.0f));
	}

	// �����հ� �޼տ� ������Ʈ �ø����� �־ ���������� �����
	//LeftHitComp = CreateDefaultSubobject<USphereComponent>(TEXT("LeftHitComp"));
	//LeftHitComp->SetupAttachment(MeshLeft);

	RightHitComp = CreateDefaultSubobject<USphereComponent>(TEXT("RightHitComp"));
	RightHitComp->SetupAttachment(MeshRight);

	//MeshLeft->SetGenerateOverlapEvents(true); // �ݸ������� �����ؾ���
	MeshRight->SetGenerateOverlapEvents(true);

	// �÷��̾� �� ����
	PlayerGunWidgetComp = CreateDefaultSubobject <UWidgetComponent>(TEXT("Player Gun Component"));
	PlayerGunWidgetComp->SetupAttachment(MeshRight);

	// �÷��̾� ��������
	PlayerShieldWidgetComp = CreateDefaultSubobject <UWidgetComponent>(TEXT("Player Shield Component"));
	PlayerShieldWidgetComp->SetupAttachment(VRCamera);

	//RightAim = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightAim"));
	//RightAim->SetupAttachment(RootComponent);
	//RightAim->SetTrackingMotionSource(TEXT("RightAim")); // ���⸦ �������� ��ǥ�踦 �����.

	
	rightScene = CreateDefaultSubobject<USceneComponent>(TEXT("player Gun Fire Target"));
	rightScene->SetupAttachment(MeshRight);
	rightScene->SetRelativeLocation(FVector(0.25f, 0.0f, 0.58f));
	rightScene->SetRelativeRotation(MeshRight->GetComponentRotation());

}

void AGunPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	// ���� ����
	PlayerWidget = Cast<UPlayerWidget>(PlayerGunWidgetComp->GetWidget());
	shieldWidget = Cast<UshieldWidget>(PlayerShieldWidgetComp->GetWidget());
	gi = Cast<UmusicGameInstance>(GetGameInstance());

	SetStartLoc();

	// �÷��̾� �̵��ӵ�
	if (GetWorld()->GetMapName().Contains(FString("LoadingLevel"))) {
		if (shieldWidget != nullptr)
		{
			shieldWidget->startShield();
		}
	}
	else if (GetWorld()->GetMapName().Contains(FString("TextLevel"))) {
		targetLoc = GetActorLocation() + FVector(1000, 0, 0);
		endTime = 3.0f;
	}
	else if (GetWorld()->GetMapName().Contains(FString("startMap")) || GetWorld()->GetMapName().Contains(FString("gameStartMap"))) {
		targetLoc = GetActorLocation();
		endTime = 1.0f;
	}
	bulletFactory = 15;

	// ��ǲ
	pc = Cast<APlayerController>(Controller);
	
	if (pc != nullptr)
	{
		auto subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subsystem)
		{
			subsystem->AddMappingContext(IMC_GunPlayer, 0);
		}
		pc->SetShowMouseCursor(true);
	}

	// ��ġ����
	RightHitComp->OnComponentBeginOverlap.AddDynamic(this, &AGunPlayer::BeginOverlap); // �ݸ������� �����ؾ���
	//MeshLeft->OnComponentBeginOverlap.AddDynamic(this, &AGunPlayer::BeginOverlap);

}

void AGunPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bStart && moveTime < endTime)
	{
		if (!bIsCastingDone) {
			for (TActorIterator<AMusicActor> iter(GetWorld()); iter; ++iter) {
				musicActor = Cast<AMusicActor>(*iter);
			}
			if (musicActor != nullptr) {
				bIsCastingDone = true;
			}
		}

		moveTime += DeltaTime;
		//UE_LOG(LogTemp, Warning, TEXT("%f / %f"), moveTime, endTime);
		SetActorLocation(FMath::Lerp(startLoc, targetLoc, (moveTime/endTime)), true);
	}
	
	if (FVector::Dist(VRCamera->GetComponentLocation(), MeshRight->GetComponentLocation()) > 70)
	{
		ONReroad();
		//UE_LOG(LogTemp, Warning, TEXT("reload"));
	}

	// �׻� ������ �� �ٶ󺸵���
	//PlayerGunWidgetComp->SetWorldRotation(BillboardWidgetComponent(this));
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
		//enhancedInputComponent->BindAction(IA_Reroad, ETriggerEvent::Started, this, &AGunPlayer::ONReroad);
	}
}

//�÷��̾� ��ŸƮ ��ġ ����
void AGunPlayer::SetStartLoc()
{
	gi = Cast<UmusicGameInstance>(GetGameInstance());
	startLoc = GetActorLocation();
	
	if (gi->bIsPlaingBBKK){
		targetLoc = GetActorLocation() + FVector(31400.0f, 0.0f, 0.0f);
		endTime = 136.0f;
		//UE_LOG(LogTemp, Warning, TEXT("%f"), endTime);
	}
	else {
		targetLoc = GetActorLocation() + FVector(31400.0f, 0.0f, 0.0f);
		endTime = 130.0f;
		//UE_LOG(LogTemp, Warning, TEXT("%f"), endTime);
	}
	//UE_LOG(LogTemp, Warning, TEXT("start : %f, %f, %f"), startLoc.X, startLoc.Y, startLoc.Z);
	//UE_LOG(LogTemp, Warning, TEXT("end : %f, %f, %f"), targetLoc.X, targetLoc.Y, targetLoc.Z);

}

// �Ѿ� �߻� ��ǲ
void AGunPlayer::ONFire(const FInputActionValue& value)
{
	FVector StartFire = MeshRight->GetComponentLocation();
	
	if (bulletFactory > 0)
	{ 
		bulletFactory -= 1;
		if (PlayerWidget != nullptr)
		{
			PlayerWidget->remainBullet(-1);
			//UE_LOG(LogTemp, Warning, TEXT("remain-1"));
		}
		if (NI_Fire != nullptr)
		{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NI_Fire, rightScene->GetComponentLocation(), rightScene->GetComponentRotation()); //���⿡�ٿ�����
		}

		//UE_LOG(LogTemp, Warning, TEXT("2222222"));
		bool isPressed = value.Get<bool>();   // .�� ĳ��Ʈ�� �ǹ� , �����ڰ� ��� ������ ������ �� ��� �̷������� �ۼ�
		if (isPressed)
		{
			FHitResult hitInfo;
			FVector start = rightScene->GetComponentLocation();
			FVector end = start + rightScene->GetForwardVector() * 100000;
			FCollisionQueryParams params;
			params.AddIgnoredActor(this);  // ���ʹ� �� �ڽ�,
			params.AddIgnoredComponent(MeshRight); // Ȥ�� �� �Ž��� ���� �� �����Ƿ�

			FQuat startRot = FRotator(0, 0, 0).Quaternion();

			// Sphere Trace
			bool bResult = GetWorld()->SweepSingleByChannel(hitInfo, start, end, startRot, ECC_Visibility, FCollisionShape::MakeSphere(60), params);
			//DrawDebugBoxTraceSingle(GetWorld(), start, end, FVector(30), FRotator(0, 0, 0), EDrawDebugTrace::ForDuration, true, hitInfo, FLinearColor::Green, FLinearColor::Red, 2.0f);

			// Sound
			if (fireSound != nullptr)
			{
				UGameplayStatics::PlaySound2D(GetWorld(), fireSound);
			}

			// ���� �ε������� �ִٸ�
			if (bResult)
			{
				enemy = Cast<ABaseEnemy>(hitInfo.GetActor());
				widgetLevel = Cast<AMainRobeUIActor>(hitInfo.GetActor());
				startActor = Cast<AgameStartActor>(hitInfo.GetActor());
					if (enemy != nullptr)
					{
						UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FX_FireHit, hitInfo.ImpactPoint, FRotator::ZeroRotator, FVector(1.0f));

						bool bIsEnemyDead = enemy->Hit(false, CurrentXEnemy);
						if (bIsEnemyDead)
						{
							CurrentXNumber++;
							SetCurrentXNumber();
							UE_LOG(LogTemp , Warning, TEXT("1"));
							if(!bshield)
							{
								shieldrecovery();
							}
						}
						return;
					}

					else if (widgetLevel != nullptr)
					{
						widgetLevel->MoveLevel();
					}

					else if (startActor != nullptr)
					{
						if (startActor->bPlayStart)
						{
							UGameplayStatics::OpenLevel(this, "startMap");
						}
					}
			}

			////UE_LOG(LogTemp, Warning, TEXT("1111111"));
			////���콺 Ŀ�� �Է°� Ȯ��
			//FVector WorldPosition, WorldDirection;
			//APlayerController* MyController = Cast<APlayerController>(GetController());
			//MyController->DeprojectMousePositionToWorld(WorldPosition, WorldDirection);
			//FHitResult hitInfo;   // ���콺�� ��Ʈ�Ǿ�����
			//
			//FCollisionQueryParams queryParams;
			//queryParams.AddIgnoredActor(player);

			//FVector endLoc = WorldPosition + WorldDirection * 10000;

			//// ������ü�� 45�� ȸ����Ų ���·� �߻��Ѵ�.
			//FQuat startRot = FRotator(0, 0, 45).Quaternion();

			//bool bResult = GetWorld()->SweepSingleByChannel(hitInfo, StartFire, endLoc, startRot, ECC_Visibility, FCollisionShape::MakeSphere(10), queryParams);

			//if (bResult)
			//{
			//	UE_LOG(LogTemp, Warning, TEXT("Hit Actor name: %s"), *hitInfo.GetActor()->GetActorNameOrLabel());
			//	FVector centerPos = (StartFire + endLoc) * 0.5f;
			//	enemy = Cast<ABaseEnemy>(hitInfo.GetActor());
			//	if (enemy != nullptr)
			//	{
			//		enemy->Hit(false);
			//		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FX_FireHit, hitInfo.ImpactPoint, FRotator::ZeroRotator, FVector(3.0f));
			//	}
			//}

			//DrawDebugBoxTraceSingle(GetWorld(), StartFire, endLoc, FVector(10), FRotator(0, 0, 45), EDrawDebugTrace::ForDuration, true, hitInfo, FLinearColor::Green, FLinearColor::Red, 2.0f);


			//if (GetWorld()->LineTraceSingleByChannel(hitInfo, WorldPosition, WorldPosition + WorldDirection * 10000, ECC_Visibility))
			//{
			//	targetPos = hitInfo.ImpactPoint;    //��Ʈ�� ��ǥ

			//	//targetPos.Z = GetActorLocation().Z;   //z ��ǥ�� �÷��̾��� ��ǥ�� ����
			//

			//	// ��ǥ ���� Ȯ��
			//	DrawDebugSphere(GetWorld(), hitInfo.ImpactPoint, 10.0f, 15, FColor::Red, false, 3, 1, 1);
			//	UE_LOG(LogTemp, Warning, TEXT("%.1f, %.1f, %.1f"), hitInfo.ImpactPoint.X, hitInfo.ImpactPoint.Y, hitInfo.ImpactPoint.Z);
			//	enemy = Cast<ABaseEnemy>(hitInfo.GetActor());
			//	if (enemy != nullptr)
			//	{
			//	enemy->Hit(false);
			//	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FX_FireHit, hitInfo.ImpactPoint, FRotator::ZeroRotator, FVector(3.0f));
			//	}
			//}
		}
	}
}

void AGunPlayer::ONTurn(const FInputActionValue& value)
{
	float v = value.Get<float>();
	AddControllerYawInput(v);
}

void AGunPlayer::ONReroad()
{
	bulletFactory = 15;
	PlayerWidget->remainBullet(15);
}

void AGunPlayer::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	enemy = Cast<ABaseEnemy>(OtherActor);
	if (OtherActor->IsA<ABaseEnemy>())
	{
		if (enemy->Hit(true, CurrentXEnemy) && bshield == false)
		{
			shieldrecovery();
		}
		CurrentXNumber++;
		SetCurrentXNumber();
		UE_LOG(LogTemp, Warning, TEXT("2"));
	}
}

// ���ݴ��ϸ�
void AGunPlayer::OnDamaged()
{
	pc = GetController<APlayerController>();
	if (bshield == true)
	{
		CurrentXEnemy = 1;
		CurrentXNumber = 0;
		shieldWidget->CurrentX(0);
		// ���̵� �� ȿ���� �ش�.
		if (pc != nullptr)
		{
			pc->PlayerCameraManager->StartCameraFade(1, 0, 1.0f, FLinearColor::Red);
			 // �Ұ����� ƽ�������ؼ� ��Ÿ�����带 ���� ���ǼҸ��� �����Ѵ�.
			musicActor->VolumeHitReact(false);
		}
		
		// ���������� ��Ʈ������������ �����Ѵ�.
		if (shieldWidget != nullptr)
		{
			shieldWidget->hitShield();
			bshield = false;
		}
		
	}
	
	else if (bshield == false)
	{
		//Destroy();
	}
}


void AGunPlayer::shieldrecovery()
{
		shieldWidget->removeShield(-1);
		recovery += 1;
		if (recovery == 4)
		{
			if (shieldWidget != nullptr)
			{
			 // ȸ�� ���̵�ƿ� ȿ���� �ش�.
				pc->PlayerCameraManager->StartCameraFade(1, 0, 1.0f, FLinearColor::Green);
				shieldWidget->produceShield();
				bshield = true;
				shieldWidget->removeShield(4);
				recovery = 0;
			}
		}
}

void AGunPlayer::SetCurrentXNumber()
{
	// ����
	if (CurrentXNumber == 2)
	{
		shieldWidget->CurrentX(1);
		CurrentXEnemy += 1;
		UE_LOG(LogTemp, Warning, TEXT("Num1"));
	}
	else if (CurrentXNumber == 6)
	{
		shieldWidget->CurrentX(2);
		CurrentXEnemy += 2;
		UE_LOG(LogTemp, Warning, TEXT("Num2"));
	}
	else if (CurrentXNumber == 14)
	{
		shieldWidget->CurrentX(4);
		CurrentXNumber += 4;
		UE_LOG(LogTemp, Warning, TEXT("Num3"));
	}
}
//FRotator AGunPlayer::BillboardWidgetComponent(AActor* camActor)
//{
//		if (VRCamera != nullptr)
//		{
//			FVector lookDir = (VRCamera->GetComponentLocation() - PlayerGunWidgetComp->GetComponentLocation()).GetSafeNormal();
//			FRotator lookRot = UKismetMathLibrary::MakeRotFromX(lookDir);
//			return lookRot;
//		}
//		else
//		{
//			return FRotator::ZeroRotator;
//		}
//}



	