// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DeathWidget.generated.h"

/**
 * 
 */
UCLASS()
class MELEECOMBATDEMO_API UDeathWidget : public UUserWidget
{
	GENERATED_BODY()
	
	TSoftObjectPtr<UWorld> LevelToLoad;
protected:
	virtual void OnAnimationFinishedPlaying(UUMGSequencePlayer& Player) override;

public:
	UFUNCTION(BlueprintCallable)
	void PlayDeathAnimation(UWidgetAnimation* AnimationToPlay, TSoftObjectPtr<UWorld> Level);
};
