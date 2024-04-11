// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BehaviorTree/BTT_MeleeAttack.h"

EBTNodeResult::Type UBTT_MeleeAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogClass, Warning, TEXT("Melee Attack"));

	return EBTNodeResult::Succeeded;
}

void UBTT_MeleeAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
}