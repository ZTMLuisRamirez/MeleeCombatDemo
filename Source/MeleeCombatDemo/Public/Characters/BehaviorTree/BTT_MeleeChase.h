// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_MeleeChase.generated.h"

/**
 * 
 */
UCLASS()
class MELEECOMBATDEMO_API UBTT_MeleeChase : public UBTTaskNode
{
	GENERATED_BODY()
	
	bool bIsFinished{ false };

	// This property should be set to 350
	// at a minimum. Otherwise, the enemy
	// will stutter during rotations.
	UPROPERTY(EditAnywhere)
	double AttackRadius{ 350.0f };

	UPROPERTY(EditAnywhere)
	float AcceptableRadius{ 100.0f };

	FScriptDelegate MoveDelegate;
public:
	UBTT_MeleeChase();

	UFUNCTION()
	void FinishChaseTask();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
