// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_MeleeAttack.generated.h"

/**
 * 
 */
UCLASS()
class MELEECOMBATDEMO_API UBTT_MeleeAttack : public UBTTaskNode
{
	GENERATED_BODY()

	FTimerHandle AttackTimerHandle;

	UFUNCTION()
	void FinishAttackTask();

	bool bIsFinished{ false };

public:
	UBTT_MeleeAttack();
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
