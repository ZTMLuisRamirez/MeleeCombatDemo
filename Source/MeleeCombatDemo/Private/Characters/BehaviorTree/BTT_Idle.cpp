// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BehaviorTree/BTT_Idle.h"
#include "AIController.h"

EBTNodeResult::Type UBTT_Idle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerComp.GetAIOwner()->StopMovement(); 
	OwnerComp.GetAIOwner()->ClearFocus(EAIFocusPriority::Gameplay); 

	return EBTNodeResult::Succeeded;
}
