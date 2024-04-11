// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_PlayerLocation.generated.h"

/**
 * 
 */
UCLASS()
class MELEECOMBATDEMO_API UBTS_PlayerLocation : public UBTService
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere)
	double Range{ 1250.0 };

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector BlackboardKey;

	virtual void TickNode (
		UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory,
		float DeltaSeconds
	) override;
};
