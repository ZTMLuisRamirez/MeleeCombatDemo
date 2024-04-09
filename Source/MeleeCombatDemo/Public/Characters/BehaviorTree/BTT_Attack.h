// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_Attack.generated.h"

/**
 * 
 */
UCLASS()
class MELEECOMBATDEMO_API UBTT_Attack : public UBTTaskNode
{
	GENERATED_BODY()

	FTimerHandle AttackTimerHandle;

	UFUNCTION()
	void FinishAttackTask();

	bool bIsFinished{ false };

	FScriptDelegate MoveDelegate;

	UPROPERTY(EditAnywhere)
	double AttackRadius{ 200.0f };

	UPROPERTY(EditAnywhere)
	float AcceptableRadius{ 100.0f };

public:
	UBTT_Attack();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
