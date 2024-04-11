// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BehaviorTree/BTT_RangedAttack.h"

EBTNodeResult::Type UBTT_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogClass, Warning, TEXT("Ranged Attack"));

	return EBTNodeResult::Succeeded;
}

void UBTT_RangedAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
}
