// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BehaviorTree/UBTTask_Patrol.h"
#include "AIController.h"
#include "Characters/PatrolComponent.h"
#include "Engine/TargetPoint.h"
#include "Navigation/PathFollowingComponent.h"

UUBTTask_Patrol::UUBTTask_Patrol()
{
	NodeName = TEXT("Patrol");

	bNotifyTick = true;
}

EBTNodeResult::Type UUBTTask_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner() == nullptr) { return EBTNodeResult::Failed; }

	UPatrolComponent* PatrolComp = OwnerComp.GetAIOwner()
		->GetPawn<AActor>()
		->FindComponentByClass<UPatrolComponent>();
	ATargetPoint* NewLocation = PatrolComp->GetCurrentPoint();

	EPathFollowingRequestResult::Type MoveResult{ 
		OwnerComp.GetAIOwner()->MoveTo(NewLocation) 
	};

	if (MoveResult == EPathFollowingRequestResult::Failed) { return EBTNodeResult::Failed; }

	bIsFinished = false;

	PatrolComp->IncrementPoint();

	OwnerComp.GetAIOwner()->GetWorldTimerManager()
		.SetTimer(
			PatrolWaitTimerHandler,
			this,
			&UUBTTask_Patrol::OnFinishWaiting,
			WaitTime,
			false
		);

	return EBTNodeResult::InProgress;
}

void UUBTTask_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!bIsFinished) { return; }

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

void UUBTTask_Patrol::OnFinishWaiting()
{
	bIsFinished = true;
}