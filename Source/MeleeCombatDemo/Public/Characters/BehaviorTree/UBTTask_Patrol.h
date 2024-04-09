// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "UBTTask_Patrol.generated.h"

/**
 * 
 */
UCLASS()
class MELEECOMBATDEMO_API UUBTTask_Patrol : public UBTTaskNode
{
	GENERATED_BODY()

	FTimerHandle PatrolWaitTimerHandler;

	UPROPERTY(EditAnywhere)
	float WaitTime{ 3.0f };

	bool bIsFinished{ false };
	
public:
	UUBTTask_Patrol();

	UFUNCTION()
	void OnFinishWaiting();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
