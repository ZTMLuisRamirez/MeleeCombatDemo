// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BehaviorTree/BTT_RangedAttack.h"
#include "GameFramework/Character.h"
#include "AIController.h"

UBTT_RangedAttack::UBTT_RangedAttack() 
{
	NodeName = TEXT("Ranged Attack");

	bNotifyTick = true;
}

EBTNodeResult::Type UBTT_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ACharacter* CharacterRef = Cast<ACharacter>(
		OwnerComp.GetAIOwner()->GetPawn()
	);

	if (CharacterRef == nullptr) { return EBTNodeResult::Failed; }
	
	float Duration = CharacterRef->PlayAnimMontage(AnimMontage);

	CharacterRef->GetWorldTimerManager().SetTimer(
		AttackTimerHandle,
		this,
		&UBTT_RangedAttack::FinishAttackTask,
		Duration,
		false
	);

	return EBTNodeResult::InProgress;
}

void UBTT_RangedAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!bIsFinished) { return; }

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

void UBTT_RangedAttack::FinishAttackTask()
{
	bIsFinished = true;
}