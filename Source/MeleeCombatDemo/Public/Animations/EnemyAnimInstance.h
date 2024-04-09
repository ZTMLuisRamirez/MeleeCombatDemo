// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnResetEnemyAttackSignature);

/**
 * 
 */
UCLASS()
class MELEECOMBATDEMO_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	APawn* ActorRef;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VerticalSpeed{ 0.f };

	virtual void NativeInitializeAnimation() override;

	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

public:
	UPROPERTY()
	FOnResetEnemyAttackSignature OnResetAttackDelegate;

	UFUNCTION(BlueprintCallable, Meta = (HideSelfPin = "true"))
	void RaiseResetAttack();
};
