// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AttackTraceAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class MELEECOMBATDEMO_API UAttackTraceAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
	
	virtual void NotifyBegin(
		USkeletalMeshComponent* MeshComp, 
		UAnimSequenceBase* Animation, 
		float TotalDuration,
		const FAnimNotifyEventReference &EventReference
	) override;

	virtual void NotifyEnd(
		USkeletalMeshComponent* MeshComp,
		UAnimSequenceBase* Animation,
		const FAnimNotifyEventReference& EventReference
	) override;

	void ToggleAttack(USkeletalMeshComponent* MeshComp, bool NewVal);
};
