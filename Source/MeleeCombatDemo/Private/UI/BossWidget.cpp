// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BossWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UBossWidget::NativeConstruct()
{
	HealthBar = Cast<UProgressBar>(GetWidgetFromName(HealthBarName));

	TextWidget = Cast<UTextBlock>(GetWidgetFromName(BossName));
}

void UBossWidget::UpdateHealthBar(float CurrentVal, float MaxVal)
{
	float Percentage{ CurrentVal / MaxVal };

	HealthBar->SetPercent(Percentage);
}

void UBossWidget::UpdateBossNameWidget(FText NewName)
{
	TextWidget->SetText(NewName);
}