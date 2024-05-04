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


AGunPlayer::AGunPlayer()
{
 
	PrimaryActorTick.bCanEverTick = true;

	// VR 카메라 컴포넌트를 생성하고 루트에 붙이고싶다.
	VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));
	VRCamera->SetupAttachment(RootComponent);

	// 박스 컴포넌트를 카메라에 붙여서 벽에 닿거나 총알에 맞으면 데미지를 받는다.
	boxcomp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	boxcomp->SetupAttachment(RootComponent);

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
		MeshRight->SetWorldLocationAndRotation(FVector((6.68f, 10.7f, 1.97f)), FRotator((75.0f, 0.0f, 180.0f)));
		MeshRight->SetRelativeScale3D(FVector(20.0f));
	}

	// 오른손과 왼손에 컴포넌트 컬리전을 넣어서 근접공격을 만든다
	//LeftHitComp = CreateDefaultSubobject<USphereComponent>(TEXT("LeftHitComp"));
	//LeftHitComp->SetupAttachment(MeshLeft);


	RightHitComp = CreateDefaultSubobject<USphereComponent>(TEXT("RightHitComp"));
	RightHitComp->SetupAttachment(MeshRight);

	//MeshLeft->SetGenerateOverlapEvents(true); // 콜리전으로 변경해야함
	MeshRight->SetGenerateOverlapEvents(true);

	PlayerGunWidgetComp = CreateDefaultSubobject <UWidgetComponent>(TEXT("Player Gun Component"));
	PlayerGunWidgetComp->SetupAttachment(MeshRight);

}


void AGunPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	startLoc = GetActorLocation();

	if (GetWorld()->GetMapName().Contains(FString("BBKKBKK"))) {
		targetLoc = GetActorLocation() + FVector(31400.0f, 0.0f, 0.0f);
		endTime = 136.0f;
	}
	else if (GetWorld()->GetMapName().Contains(FString("NightTheater"))) {
		targetLoc = GetActorLocation() + FVector(31400.0f, 0.0f, 0.0f);
		endTime = 130.0f;
	}
	else {
		targetLoc = GetActorLocation() + FVector(1000, 0, 0);
		endTime = 3.0f;
	}

	bulletFactory = 15;

	// 플레이어컨트롤러를 가져오고싶다.
	auto* pc = Cast<APlayerController>(Controller); // AVRPlayer의 컨트롤러를 가져오는것

	// UEnhancedInputLocalPlayerSubsystem를 가져와서
	if (pc != nullptr)
	{
		auto subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(pc->GetLocalPlayer());
		if (subsystem)
		{
			// AddMappingContext를 호출하고싶다.
			subsystem->AddMappingContext(IMC_GunPlayer, 0);
		}
		pc->SetShowMouseCursor(true);
	}

	
	MeshRight->OnComponentBeginOverlap.AddDynamic(this, &AGunPlayer::BeginOverlap); // 콜리전으로 변경해야함
	//MeshLeft->OnComponentBeginOverlap.AddDynamic(this, &AGunPlayer::BeginOverlap);

	PlayerWidget = Cast<UPlayerWidget>(PlayerGunWidgetComp->GetWidget());

}




void AGunPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	moveTime += DeltaTime;

	if (moveTime < endTime)
	{
		//UE_LOG(LogTemp,Warning,TEXT("11111"));
		SetActorLocation(FMath::Lerp(startLoc, targetLoc, (moveTime/endTime)), true);
	}
	/*else
	{
		moveTime = 0;
		UE_Log(LogTemp, warning, TEXT("%s"), Score);

	}*/

	/*if (BaseEnemy->Perpect)
	{
		Score += 400;
		BaseEnemy->Perpect = false;
	}*/

	// 플레이어 나이아가라 위치
	/*playerfireLoc = GetActorLocation()+FVector(100,0,0); 
	playerfireRot = GetActorRotation();*/

	if (bshield == false)
	{
		shieldrecovery();
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
	}

}


// 총알 발사 인풋
void AGunPlayer::ONFire(const FInputActionValue& value)
{
	
	
	if (bulletFactory > 0)
	{ 
		if (PlayerWidget != nullptr)
		{
			PlayerWidget->remainBullet(-1);
			//UE_LOG(LogTemp, Warning, TEXT("remain-1"));
		}
		if (NI_Fire != nullptr)
		{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NI_Fire, MeshRight->GetComponentLocation(),MeshRight->GetComponentRotation()); //무기에붙여야함
		}

		//UE_LOG(LogTemp, Warning, TEXT("2222222"));
		bool isPressed = value.Get<bool>();   // .이 캐스트의 의미 , 개발자가 어떻게 만들지 예측할 수 없어서 이런식으로 작성
		if (isPressed)
		{
			//UE_LOG(LogTemp, Warning, TEXT("1111111"));
			//마우스 커서 입력값 확인
			FVector WorldPosition, WorldDirection;
			APlayerController* MyController = Cast<APlayerController>(GetController());
			MyController->DeprojectMousePositionToWorld(WorldPosition, WorldDirection);
			FHitResult hitInfo;   // 마우스가 히트되었을때
			
			/*bool SphereTraceSingleForObjects
			(
				UObject * WorldContextObject,
				const FVector Start, // 시작지점
				const FVector End, // 끝지점
				float Radius, // 거리?
				const TArray < TEnumAsByte < EObjectTypeQuery > > &ObjectTypes,
				bool bTraceComplex,
				const TArray < AActor* > &ActorsToIgnore, // 이그노어처리할액터
				EDrawDebugTrace::Type DrawDebugType,
				FHitResult & OutHit, // 시작지점, 끝지점?
				bool bIgnoreSelf, // 자기자신 이그노어처리?
				FLinearColor TraceColor, // 색깔
				FLinearColor TraceHitColor, // 맞은색깔
				float DrawTime // 몇초동안그릴지
			);*/


			if (GetWorld()->LineTraceSingleByChannel(hitInfo, WorldPosition, WorldPosition + WorldDirection * 10000, ECC_Visibility))
			{
				targetPos = hitInfo.ImpactPoint;    //히트된 좌표

				//targetPos.Z = GetActorLocation().Z;   //z 좌표를 플레이어의 좌표로 설정
			

				// 좌표 설정 확인
				DrawDebugSphere(GetWorld(), hitInfo.ImpactPoint, 10.0f, 15, FColor::Red, false, 3, 1, 1);
				UE_LOG(LogTemp, Warning, TEXT("%.1f, %.1f, %.1f"), hitInfo.ImpactPoint.X, hitInfo.ImpactPoint.Y, hitInfo.ImpactPoint.Z);
				enemy = Cast<ABaseEnemy>(hitInfo.GetActor());
				if (enemy != nullptr)
				{
				enemy->Hit();
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FX_FireHit, hitInfo.ImpactPoint, FRotator::ZeroRotator, FVector(3.0f));
				}
			}
		}
	}
	bulletFactory += -1;
	
}


void AGunPlayer::ONTurn(const FInputActionValue& value)
{
	float v = value.Get<float>();
	AddControllerYawInput(v);
}

void AGunPlayer::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<ABaseEnemy>())
	{
		enemy->Hit();
	}

}


void AGunPlayer::OnDamaged()
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

void AGunPlayer::shieldrecovery()
{
	/*if (enemy->Hit());
	{
		recovery += 1;
		if (recovery == 3)
		{
			bshield = true;
		}
	}*/

	
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



	