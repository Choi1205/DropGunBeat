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

	// VR 카메라 컴포넌트를 생성하고 루트에 붙이고싶다.
	VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));
	VRCamera->SetupAttachment(RootComponent);

	// 박스 컴포넌트를 카메라에 붙여서 벽에 닿거나 총알에 맞으면 데미지를 받는다.
	boxcomp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	boxcomp->SetupAttachment(VRCamera);

	// 모션컨트롤러 왼손, 오른손 생성하고 루트에 붙이고 싶다.
	//MotionLeft = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionLeft"));
	//MotionLeft->SetTrackingMotionSource(TEXT("Left"));
	//MotionLeft->SetupAttachment(RootComponent);

	MotionRight = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("MotionRight"));
	MotionRight->SetTrackingMotionSource(TEXT("Right"));
	MotionRight->SetupAttachment(RootComponent);

	// 왼손, 오른손 스켈레탈메시컴포넌트를 만들어서 모션컨트롤러에 붙이고싶다.
	//MeshLeft = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshLeft"));
	//MeshLeft->SetupAttachment(MotionLeft);
	MeshRight = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshRight"));
	MeshRight->SetupAttachment(MotionRight);

	// 왼손, 오른손 스켈레탈 메시를로드해서 적용하고싶다.
	//ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMeshLeft(TEXT("/Script/Engine.SkeletalMesh'/Game/assets/pistol-desert-eagle-weapon-model-cs2/DEAGLE.DEAGLE'"));
	//// 로드 성공했다면 적용하고싶다.
	//if (TempMeshLeft.Succeeded())
	//{
	//	MeshLeft->SetSkeletalMesh(TempMeshLeft.Object);
	//	MeshLeft->SetWorldLocationAndRotation(FVector((6.7f,-11.25f,1.97f)), FRotator((75.0f,  0.0f,  180.0f)));
	//	MeshLeft->SetRelativeScale3D(FVector(20.0f));
	//}

	ConstructorHelpers::FObjectFinder<USkeletalMesh> TempMeshRight(TEXT("/Script/Engine.SkeletalMesh'/Game/assets/pistol-desert-eagle-weapon-model-cs2/DEAGLE.DEAGLE'"));
	// 로드 성공했다면 적용하고싶다.
	if (TempMeshRight.Succeeded())
	{
		MeshRight->SetSkeletalMesh(TempMeshRight.Object);
		MeshRight->SetWorldLocationAndRotation(FVector((13.3f, 10.7f, -25.1f)), FRotator((35.0f, 0.0f, 180.0f)));
		MeshRight->SetRelativeScale3D(FVector(20.0f));
	}

	// 오른손과 왼손에 컴포넌트 컬리전을 넣어서 근접공격을 만든다
	//LeftHitComp = CreateDefaultSubobject<USphereComponent>(TEXT("LeftHitComp"));
	//LeftHitComp->SetupAttachment(MeshLeft);

	RightHitComp = CreateDefaultSubobject<USphereComponent>(TEXT("RightHitComp"));
	RightHitComp->SetupAttachment(MeshRight);

	//MeshLeft->SetGenerateOverlapEvents(true); // 콜리전으로 변경해야함
	MeshRight->SetGenerateOverlapEvents(true);

	// 플레이어 건 위젯
	PlayerGunWidgetComp = CreateDefaultSubobject <UWidgetComponent>(TEXT("Player Gun Component"));
	PlayerGunWidgetComp->SetupAttachment(MeshRight);

	// 플레이어 쉴드위젯
	PlayerShieldWidgetComp = CreateDefaultSubobject <UWidgetComponent>(TEXT("Player Shield Component"));
	PlayerShieldWidgetComp->SetupAttachment(VRCamera);

	//RightAim = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightAim"));
	//RightAim->SetupAttachment(RootComponent);
	//RightAim->SetTrackingMotionSource(TEXT("RightAim")); // 여기를 기준으로 좌표계를 사용함.

	
	rightScene = CreateDefaultSubobject<USceneComponent>(TEXT("player Gun Fire Target"));
	rightScene->SetupAttachment(MeshRight);
	rightScene->SetRelativeLocation(FVector(0.25f, 0.0f, 0.58f));
	rightScene->SetRelativeRotation(MeshRight->GetComponentRotation());

}

void AGunPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	// 위젯 연결
	PlayerWidget = Cast<UPlayerWidget>(PlayerGunWidgetComp->GetWidget());
	shieldWidget = Cast<UshieldWidget>(PlayerShieldWidgetComp->GetWidget());
	gi = Cast<UmusicGameInstance>(GetGameInstance());

	SetStartLoc();

	// 플레이어 이동속도
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

	// 인풋
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

	// 펀치공격
	RightHitComp->OnComponentBeginOverlap.AddDynamic(this, &AGunPlayer::BeginOverlap); // 콜리전으로 변경해야함
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

	// 항상 위젯이 날 바라보도록
	//PlayerGunWidgetComp->SetWorldRotation(BillboardWidgetComponent(this));
}

void AGunPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (enhancedInputComponent != nullptr)
	{
		// 함수를 인풋 컴포넌트에 연결한다.
		enhancedInputComponent->BindAction(IA_Fire, ETriggerEvent::Started, this, &AGunPlayer::ONFire);
		enhancedInputComponent->BindAction(IA_Turn, ETriggerEvent::Triggered, this, &AGunPlayer::ONTurn);
		//enhancedInputComponent->BindAction(IA_Reroad, ETriggerEvent::Started, this, &AGunPlayer::ONReroad);
	}
}

//플레이어 스타트 위치 지정
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

// 총알 발사 인풋
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
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NI_Fire, rightScene->GetComponentLocation(), rightScene->GetComponentRotation()); //무기에붙여야함
		}

		//UE_LOG(LogTemp, Warning, TEXT("2222222"));
		bool isPressed = value.Get<bool>();   // .이 캐스트의 의미 , 개발자가 어떻게 만들지 예측할 수 없어서 이런식으로 작성
		if (isPressed)
		{
			FHitResult hitInfo;
			FVector start = rightScene->GetComponentLocation();
			FVector end = start + rightScene->GetForwardVector() * 100000;
			FCollisionQueryParams params;
			params.AddIgnoredActor(this);  // 액터는 나 자신,
			params.AddIgnoredComponent(MeshRight); // 혹시 내 매쉬에 닿을 수 있으므로

			FQuat startRot = FRotator(0, 0, 0).Quaternion();

			// Sphere Trace
			bool bResult = GetWorld()->SweepSingleByChannel(hitInfo, start, end, startRot, ECC_Visibility, FCollisionShape::MakeSphere(60), params);
			//DrawDebugBoxTraceSingle(GetWorld(), start, end, FVector(30), FRotator(0, 0, 0), EDrawDebugTrace::ForDuration, true, hitInfo, FLinearColor::Green, FLinearColor::Red, 2.0f);

			// Sound
			if (fireSound != nullptr)
			{
				UGameplayStatics::PlaySound2D(GetWorld(), fireSound);
			}

			// 만약 부딪힌것이 있다면
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
			////마우스 커서 입력값 확인
			//FVector WorldPosition, WorldDirection;
			//APlayerController* MyController = Cast<APlayerController>(GetController());
			//MyController->DeprojectMousePositionToWorld(WorldPosition, WorldDirection);
			//FHitResult hitInfo;   // 마우스가 히트되었을때
			//
			//FCollisionQueryParams queryParams;
			//queryParams.AddIgnoredActor(player);

			//FVector endLoc = WorldPosition + WorldDirection * 10000;

			//// 정육면체를 45도 회전시킨 상태로 발사한다.
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
			//	targetPos = hitInfo.ImpactPoint;    //히트된 좌표

			//	//targetPos.Z = GetActorLocation().Z;   //z 좌표를 플레이어의 좌표로 설정
			//

			//	// 좌표 설정 확인
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

// 공격당하면
void AGunPlayer::OnDamaged()
{
	pc = GetController<APlayerController>();
	if (bshield == true)
	{
		CurrentXEnemy = 1;
		CurrentXNumber = 0;
		shieldWidget->CurrentX(0);
		// 페이드 인 효과를 준다.
		if (pc != nullptr)
		{
			pc->PlayerCameraManager->StartCameraFade(1, 0, 1.0f, FLinearColor::Red);
			 // 불값으로 틱에전달해서 델타세컨드를 통해 음악소리를 조절한다.
			musicActor->VolumeHitReact(false);
		}
		
		// 쉴드위젯을 히트쉴드위젯으로 변경한다.
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
			 // 회복 페이드아웃 효과를 준다.
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
	// 점수
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



	