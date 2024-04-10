// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DeathWidget.h"
#include "Kismet/GameplayStatics.h"

void UDeathWidget::PlayDeathAnimation(UWidgetAnimation* AnimationToPlay, TSoftObjectPtr<UWorld> Level)
{
	LevelToLoad = Level;

	PlayAnimation(AnimationToPlay);
}

void UDeathWidget::OnAnimationFinishedPlaying(UUMGSequencePlayer& Player)
{
	Super::OnAnimationFinishedPlaying(Player);

	UGameplayStatics::OpenLevelBySoftObjectPtr(
		this,
		LevelToLoad,
		true
	);
}