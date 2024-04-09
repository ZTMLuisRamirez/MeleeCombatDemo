// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BehaviorTree/BTT_Attack.h"
#include "AIController.h"
#include "Interfaces/Combat.h"

UBTT_Attack::UBTT_Attack()
{
	NodeName = TEXT("Attack");
	bNotifyTick = true;

	MoveDelegate.BindUFunction(this, "FinishAttackTask");
}

EBTNodeResult::Type UBTT_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner() == nullptr) { return EBTNodeResult::Failed; }

	bIsFinished = false;

	APawn* PawnRef = OwnerComp.GetAIOwner()->GetPawn();
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
		//MoveRequest.SetGoalLocation(PlayerLocation);
		MoveRequest.SetAcceptanceRadius(AcceptableRadius);

		EPathFollowingRequestResult::Type MoveResult{
			OwnerComp.GetAIOwner()->MoveTo(MoveRequest)
		};

		OwnerComp.GetAIOwner()->SetFocus(
			GetWorld()->GetFirstPlayerController()->GetPawn()
		);
		OwnerComp.GetAIOwner()->ReceiveMoveCompleted.AddUnique(MoveDelegate);
	}
	else
	{
		ICombat* CombatRef = Cast<ICombat>(PawnRef);

		if (!CombatRef) { return EBTNodeResult::Failed; }

		CombatRef->Attack();

		PawnRef->GetWorldTimerManager().SetTimer(
			AttackTimerHandle,
			this,
			&UBTT_Attack::FinishAttackTask,
			CombatRef->GetAnimDuration(),
			false
		);
	}

	return EBTNodeResult::InProgress;
}

void UBTT_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!bIsFinished) { return; }

	if (OwnerComp.GetAIOwner()->ReceiveMoveCompleted.Contains(MoveDelegate))
	{
		OwnerComp.GetAIOwner()->ReceiveMoveCompleted.Remove(MoveDelegate);
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

void UBTT_Attack::FinishAttackTask()
{
	bIsFinished = true;
}