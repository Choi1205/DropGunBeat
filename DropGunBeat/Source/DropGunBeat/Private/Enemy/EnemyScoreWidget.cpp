// Fill out your copyright notice in the Description page of Project Settings.
//에너미가 총에 맞았을때 머리위로 뜨는 점수위젯.
//연속해서 쏘면 이전값이 지워지고 다음 값이 나타난다.

#include "Enemy/EnemyScoreWidget.h"
#include "Components/TextBlock.h"

//점수표시하는 함수. 정확도에 따른 점수와 배율을 인자값으로 받는다.
void UEnemyScoreWidget::ShowScore(int32 score, int32 multi)
{
	//점수 * 배율을 텍스트로 입력
	scoreBlock->SetText(FText::FromString(FString::FromInt(score * multi)));

	//점수에 따라 글자색을 변경
	if (score >= 400) {
		scoreBlock->SetColorAndOpacity(FColor::Red);
	}
	else if (score == 300) {
		scoreBlock->SetColorAndOpacity(FColor::Yellow);
	}
	else{
		scoreBlock->SetColorAndOpacity(FColor::White);
	}

	//점수를 표시
	scoreBlock->SetVisibility(ESlateVisibility::Visible);

	//이미 점수가 표시되는 중이라면 점수표시 중지
	if (GetWorld()->GetTimerManager().IsTimerActive(showingTimer)) {
		GetWorld()->GetTimerManager().ClearTimer(showingTimer);
		StopAnimation(scoreRise);
	}
	//새로운 점수를 표시. 애니메이션은 점수가 위로 서서히 떠오르는 애니메이션
	PlayAnimation(scoreRise);

	//3초뒤 점수표시를 없앤다.
	GetWorld()->GetTimerManager().SetTimer(showingTimer, FTimerDelegate::CreateLambda([&]() {
		scoreBlock->SetVisibility(ESlateVisibility::Hidden);
		}), 3.0f, false);
}
