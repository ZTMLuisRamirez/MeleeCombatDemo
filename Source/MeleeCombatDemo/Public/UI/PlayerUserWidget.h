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

protected:
	void NativeConstruct();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStatsComponent* StatsComp;

public:
	UFUNCTION(BlueprintCallable)
	void UpdateStaminaProgressBar(float CurrentVal, float MaxVal);

	UFUNCTION(BlueprintCallable)
	void AdjustHealth(float CurrentVal, float MaxVal);
};
