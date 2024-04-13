// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BossWidget.generated.h"

/**
 * 
 */
UCLASS()
class MELEECOMBATDEMO_API UBossWidget : public UUserWidget
{
	GENERATED_BODY()
	
	class UProgressBar* HealthBar; 

	class UStatsComponent* StatsComp;

	UPROPERTY(EditAnywhere)
	FName HealthBarName;

protected:
	void NativeConstruct();

public: 
	UFUNCTION(BlueprintCallable)
	void UpdateHealthBar(float CurrentVal, float MaxVal);
};
