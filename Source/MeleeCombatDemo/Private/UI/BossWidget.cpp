// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/BossWidget.h"
#include "Components/ProgressBar.h"

void UBossWidget::NativeConstruct()
{
	HealthBar = Cast<UProgressBar>(GetWidgetFromName(HealthBarName));
}

void UBossWidget::UpdateHealthBar(float CurrentVal, float MaxVal)
{
	float Percentage{ CurrentVal / MaxVal };

	HealthBar->SetPercent(Percentage);
}