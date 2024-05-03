// Fill out your copyright notice in the Description page of Project Settings.


#include "musicGameModeBase.h"
#include "PlayerWidget.h"
#include <../../../../../../../Source/Runtime/UMG/Public/Components/TextBlock.h>

void AmusicGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	//���� ������ �Ҵ�Ǿ� �ִٸ�, �� ���� ������ �ν��Ͻ��� ����
	if (PlayerWidget_BP != nullptr)
	{
		playerUI = CreateWidget<UPlayerWidget>(GetWorld(), PlayerWidget_BP);

		// ���� ������ ����� ���� �Ǿ��ٸ� ,���� �ν��Ͻ��� ����Ʈ�� �߰�
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
	// ���� �������� bulletCount��ŭ�� ����
	bullet += bulletCount;

	if (playerUI != nullptr)
	{
		FString bulletString = FString::Printf(TEXT(" % d"), bullet);
		FText bulletText = FText::FromString(bulletString);

		playerUI->CurrentBullet->SetText(bulletText);

	}

}


