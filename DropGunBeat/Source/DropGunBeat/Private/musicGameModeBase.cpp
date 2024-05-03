// Fill out your copyright notice in the Description page of Project Settings.


#include "musicGameModeBase.h"
#include "PlayerWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>

void AmusicGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	//위젯 파일이 할당되어 있다면, 그 위젯 파일을 인스턴스로 생성
	if (PlayerWidget_BP != nullptr)
	{
		playerUI = CreateWidget<UPlayerWidget>(GetWorld(), PlayerWidget_BP);

		// 위젯 파일이 제대로 생성 되었다면 ,위젯 인스턴스를 뷰포트에 추가
		if (playerUI != nullptr)
		{
			playerUI->AddToViewport();
		}
	}
	playerUI->CurrentBullet->SetText(FText::FromString("15"));
}


void AmusicGameModeBase::AddPoint(int32 count)
{
	


}

void AmusicGameModeBase::remainBullet(int32 bulletCount)
{
	// 현재 점수에서 bulletCount만큼을 빼기
	bullet += bulletCount;

	if (playerUI != nullptr)
	{
		FString bulletString = FString::Printf(TEXT(" % d"), bullet);
		FText bulletText = FText::FromString(bulletString);

		playerUI->CurrentBullet->SetText(bulletText);

	}

}


