// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BehaviorTree/BTT_RangedAttack.h"
#include "GameFramework/Character.h"
#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"

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
	
	bIsFinished = false;
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

	float RandomVal{ 
		static_cast<float>(UKismetMathLibrary::RandomFloat()) 
	};
	float Threshold{
		OwnerComp.GetBlackboardComponent()
			->GetValueAsFloat(TEXT("ChargeThreshold"))
	};

	if (RandomVal > Threshold)
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(
			TEXT("IsCharging"),
			true
		);

		OwnerComp.GetBlackboardComponent()->SetValueAsFloat(
			TEXT("ChargeThreshold"),
			0.9f
		);
	}
	else
	{
		Threshold -= 0.1f;

		OwnerComp.GetBlackboardComponent()->SetValueAsFloat(
			TEXT("ChargeThreshold"),
			Threshold
		);
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

void UBTT_RangedAttack::FinishAttackTask()
{
	bIsFinished = true;
}