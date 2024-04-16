// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BehaviorTree/BTT_MeleeAttack.h"
#include "AIController.h"
#include "Interfaces/Enemy.h"

UBTT_MeleeAttack::UBTT_MeleeAttack()
{
	NodeName = TEXT("Attack");
	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIController{ OwnerComp.GetAIOwner() };

	if (AIController == nullptr) { return EBTNodeResult::Failed; }

	bIsFinished = false;

	APawn* PawnRef = AIController->GetPawn();
	IEnemy* CombatRef = Cast<IEnemy>(PawnRef);

	if (!CombatRef) { return EBTNodeResult::Failed; }

	CombatRef->Attack();

	PawnRef->GetWorldTimerManager().SetTimer(
		AttackTimerHandle,
		this,
		&UBTT_MeleeAttack::FinishAttackTask,
		CombatRef->GetAnimDuration(),
		false
	);

	return EBTNodeResult::InProgress;
}

void UBTT_MeleeAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!bIsFinished) { return; }

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

void UBTT_MeleeAttack::FinishAttackTask()
{
	bIsFinished = true;
}