// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_ChargeAttack.generated.h"

/**
 * 
 */
UCLASS()
class MELEECOMBATDEMO_API UBTT_ChargeAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	ACharacter* CharacterRef;

	AAIController* AIController;

	class UBossAnimInstance* BossAnim;
	
	UPROPERTY(EditAnywhere)
	float AcceptableRadius{ 100.0f };

	FScriptDelegate ChargeCompleteDelegate;

	bool bIsFinished{ false };

protected:
	UPROPERTY(EditAnywhere)
	UAnimMontage* MoveAnim;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

public:
	UBTT_ChargeAttack();

	UFUNCTION()
	void ChargeAtPlayer();

	UFUNCTION()
	void HandleChargeComplete();

	UFUNCTION()
	void FinishAttackTask();
};
