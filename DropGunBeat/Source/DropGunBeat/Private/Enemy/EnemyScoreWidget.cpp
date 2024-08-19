// Fill out your copyright notice in the Description page of Project Settings.
//���ʹ̰� �ѿ� �¾����� �Ӹ����� �ߴ� ��������.
//�����ؼ� ��� �������� �������� ���� ���� ��Ÿ����.

#include "Enemy/EnemyScoreWidget.h"
#include "Components/TextBlock.h"

//����ǥ���ϴ� �Լ�. ��Ȯ���� ���� ������ ������ ���ڰ����� �޴´�.
void UEnemyScoreWidget::ShowScore(int32 score, int32 multi)
{
	//���� * ������ �ؽ�Ʈ�� �Է�
	scoreBlock->SetText(FText::FromString(FString::FromInt(score * multi)));

	//������ ���� ���ڻ��� ����
	if (score >= 400) {
		scoreBlock->SetColorAndOpacity(FColor::Red);
	}
	else if (score == 300) {
		scoreBlock->SetColorAndOpacity(FColor::Yellow);
	}
	else{
		scoreBlock->SetColorAndOpacity(FColor::White);
	}

	//������ ǥ��
	scoreBlock->SetVisibility(ESlateVisibility::Visible);

	//�̹� ������ ǥ�õǴ� ���̶�� ����ǥ�� ����
	if (GetWorld()->GetTimerManager().IsTimerActive(showingTimer)) {
		GetWorld()->GetTimerManager().ClearTimer(showingTimer);
		StopAnimation(scoreRise);
	}
	//���ο� ������ ǥ��. �ִϸ��̼��� ������ ���� ������ �������� �ִϸ��̼�
	PlayAnimation(scoreRise);

	//3�ʵ� ����ǥ�ø� ���ش�.
	GetWorld()->GetTimerManager().SetTimer(showingTimer, FTimerDelegate::CreateLambda([&]() {
		scoreBlock->SetVisibility(ESlateVisibility::Hidden);
		}), 3.0f, false);
}
