// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BehaviorTree/BTT_ChargeAttack.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "Animations/BossAnimInstance.h"
#include "Interfaces/Enemy.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTT_ChargeAttack::UBTT_ChargeAttack()
{
	NodeName = TEXT("Charge Attack");

	bNotifyTick = true;

	ChargeCompleteDelegate.BindUFunction(this, "HandleChargeComplete");
}

EBTNodeResult::Type UBTT_ChargeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AIController = OwnerComp.GetAIOwner();
	CharacterRef = AIController->GetCharacter();

	BossAnim = Cast<UBossAnimInstance>(  
		CharacterRef->GetMesh() 
			->GetAnimInstance() 
	);

	BossAnim->bIsCharging = true;
	bIsFinished = false;

	FTimerHandle InitChargeTimeHandler;

	OwnerComp.GetAIOwner()->GetPawn()
		->GetWorldTimerManager().SetTimer(
			InitChargeTimeHandler,
			this,
			&UBTT_ChargeAttack::ChargeAtPlayer,
			2.2f,
			false
		);

	return EBTNodeResult::InProgress;
}

void UBTT_ChargeAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!bIsFinished) { return; }

	if (AIController->ReceiveMoveCompleted.Contains(ChargeCompleteDelegate))
	{
		AIController->ReceiveMoveCompleted.Remove(ChargeCompleteDelegate);
	}

	AIController->GetBlackboardComponent()->SetValueAsBool(
		TEXT("IsCharging"),
		false
	);

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

void UBTT_ChargeAttack::ChargeAtPlayer()
{
	FVector PlayerLoc = GetWorld()->GetFirstPlayerController()
		->GetPawn()
		->GetActorLocation();

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalLocation(PlayerLoc);
	MoveRequest.SetUsePathfinding(true);
	MoveRequest.SetAcceptanceRadius(AcceptableRadius);

	AIController->MoveTo(MoveRequest);

	AIController->SetFocus(
		GetWorld()->GetFirstPlayerController()->GetPawn()
	);
	AIController->ReceiveMoveCompleted.AddUnique(ChargeCompleteDelegate);

	// TO DO: Work on Speed
	IEnemy* CombatRef = Cast<IEnemy>(CharacterRef);
}

void UBTT_ChargeAttack::HandleChargeComplete()
{
	BossAnim->bIsCharging = false;

	FTimerHandle AttackTimeHandler;

	CharacterRef->GetWorldTimerManager().SetTimer(
		AttackTimeHandler,
		this,
		&UBTT_ChargeAttack::FinishAttackTask,
		1.0f,
		false
	);
}

void UBTT_ChargeAttack::FinishAttackTask()
{
	bIsFinished = true;
}