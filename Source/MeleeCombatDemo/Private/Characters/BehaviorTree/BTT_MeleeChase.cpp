// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BehaviorTree/BTT_MeleeChase.h"
#include "AIController.h"

UBTT_MeleeChase::UBTT_MeleeChase()
{
	NodeName = TEXT("Melee Chase");
	bNotifyTick = true;

	MoveDelegate.BindUFunction(this, "FinishChaseTask");
}

EBTNodeResult::Type UBTT_MeleeChase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	if (Distance < AttackRadius) { return EBTNodeResult::Succeeded; }

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

	return EBTNodeResult::InProgress;
}

void UBTT_MeleeChase::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!bIsFinished) { return; }

	if (OwnerComp.GetAIOwner()->ReceiveMoveCompleted.Contains(MoveDelegate))
	{
		OwnerComp.GetAIOwner()->ReceiveMoveCompleted.Remove(MoveDelegate);
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

EBTNodeResult::Type UBTT_MeleeChase::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController{ OwnerComp.GetAIOwner() };

	AIController->StopMovement();

	if (OwnerComp.GetAIOwner()->ReceiveMoveCompleted.Contains(MoveDelegate))
	{
		OwnerComp.GetAIOwner()->ReceiveMoveCompleted.Remove(MoveDelegate);
	}

	return EBTNodeResult::Aborted;
}

void UBTT_MeleeChase::FinishChaseTask()
{
	bIsFinished = true;
}