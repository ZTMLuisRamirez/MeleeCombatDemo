// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BehaviorTree/BTT_MeleeAttack.h"
#include "AIController.h"
#include "Interfaces/Enemy.h"

UBTT_MeleeAttack::UBTT_MeleeAttack()
{
	NodeName = TEXT("Attack");
	bNotifyTick = true;

	MoveDelegate.BindUFunction(this, "FinishAttackTask");
}

EBTNodeResult::Type UBTT_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIController{ OwnerComp.GetAIOwner() };

	if (AIController == nullptr) { return EBTNodeResult::Failed; }

	bIsFinished = false;

	APawn* PawnRef = AIController->GetPawn();
	FVector EnemyLocation = PawnRef->GetActorLocation();
	FVector PlayerLocation = GetWorld()->GetFirstPlayerController()
		->GetPawn()
		->GetActorLocation();

	double Distance = FVector::Dist(EnemyLocation, PlayerLocation);

	if (Distance > AttackRadius)
	{
		FAIMoveRequest MoveRequest;
		MoveRequest.SetGoalActor(GetWorld()->GetFirstPlayerController()->GetPawn());
		MoveRequest.SetUsePathfinding(true);
		MoveRequest.SetAcceptanceRadius(AcceptableRadius);

		EPathFollowingRequestResult::Type MoveResult{
			AIController->MoveTo(MoveRequest)
		};

		AIController->SetFocus(
			GetWorld()->GetFirstPlayerController()->GetPawn()
		);
		AIController->ReceiveMoveCompleted.AddUnique(MoveDelegate); 
	}
	else
	{
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
	}

	return EBTNodeResult::InProgress;
}

void UBTT_MeleeAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!bIsFinished) { return; }

	if (OwnerComp.GetAIOwner()->ReceiveMoveCompleted.Contains(MoveDelegate))
	{
		OwnerComp.GetAIOwner()->ReceiveMoveCompleted.Remove(MoveDelegate);
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

EBTNodeResult::Type UBTT_MeleeAttack::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController{ OwnerComp.GetAIOwner() };

	AIController->StopMovement();

	if (OwnerComp.GetAIOwner()->ReceiveMoveCompleted.Contains(MoveDelegate))
	{
		OwnerComp.GetAIOwner()->ReceiveMoveCompleted.Remove(MoveDelegate);
	}

	return EBTNodeResult::Aborted;
}

void UBTT_MeleeAttack::FinishAttackTask()
{
	bIsFinished = true;
}