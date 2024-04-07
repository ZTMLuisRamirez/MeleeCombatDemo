// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerUserWidget.h"

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