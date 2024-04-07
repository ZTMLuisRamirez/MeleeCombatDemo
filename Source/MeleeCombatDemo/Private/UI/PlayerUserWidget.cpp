// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerUserWidget.h"
#include "Characters/StatsComponent.h"

void UPlayerUserWidget::NativeConstruct()
{
	StaminaProgressBar = Cast<UProgressBar>(
		GetWidgetFromName("StaminaProgressBar")
	);

	StatsComp = GetWorld()->GetFirstPlayerController()
		->GetPawn()
		->GetComponentByClass<UStatsComponent>();

	if (IsValid(StatsComp)) 
	{
		StatsComp->OnUpdateStatDelegate.AddDynamic(
			this, &UPlayerUserWidget::UpdateStaminaProgressBar
		);
	}
}

void UPlayerUserWidget::UpdateStaminaProgressBar(StatType TargetStat, float NewVal)
{
	if (TargetStat != StatType::Stamina) { return; }

	float Percentage{
		StatsComp->Stats[StatType::Stamina] / StatsComp->Stats[StatType::MaxStamina]
	};

	StaminaProgressBar->SetPercent(Percentage);
}

