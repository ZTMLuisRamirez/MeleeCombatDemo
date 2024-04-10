// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ProgressBar.h"
#include "Blueprint/UserWidget.h"
#include "PlayerUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class MELEECOMBATDEMO_API UPlayerUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UProgressBar* StaminaProgressBar;

	class UStatsComponent* StatsComp;
protected:
	void NativeConstruct();

public:
	UFUNCTION()
	void UpdateStaminaProgressBar(enum StatType TargetStat, float NewVal);

	void AdjustHealth(float CurrentVal, float MaxVal);
};
