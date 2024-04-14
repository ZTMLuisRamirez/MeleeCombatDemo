// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerUserWidget.h"
#include "Characters/StatsComponent.h"
#include "Components/ProgressBar.h"

void UPlayerUserWidget::NativeConstruct()
{
	StaminaProgressBar = Cast<UProgressBar>(
		GetWidgetFromName("StaminaProgressBar")
	);
}

void UPlayerUserWidget::UpdateStaminaProgressBar(float CurrentVal, float MaxVal)
{
	float Percentage{ CurrentVal / MaxVal };

	StaminaProgressBar->SetPercent(Percentage);
}

void UPlayerUserWidget::AdjustHealth(float CurrentVal, float MaxVal)
{
	UProgressBar* HealthProgressBar = Cast<UProgressBar>(
		GetWidgetFromName("HealthProgressBar")
	);

	float Percentage{ CurrentVal / MaxVal };

	HealthProgressBar->SetPercent(Percentage);
}