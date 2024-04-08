// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnResetAttackComboSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnResetAttackSignature);

/**
 * 
 */
UCLASS()
class MELEECOMBATDEMO_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
	APawn* PawnRef;

protected:
	virtual void NativeInitializeAnimation() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VerticalSpeed{ 0.f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsFalling{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Direction{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsInCombat{ false };

public:
	UPROPERTY(BlueprintAssignable)
	FOnResetAttackComboSignature OnResetAttackComboDelegate;

	UPROPERTY()
	FOnResetAttackSignature OnResetAttackDelegate;

	UFUNCTION(BlueprintCallable, Meta = (HideSelfPin = "true"))
	void UpdateSpeedWithVelocity();

	UFUNCTION(BlueprintCallable, Meta = (HideSelfPin = "true"))
	void UpdateFalling();

	UFUNCTION(BlueprintCallable)
	void HandleUpdatedTarget(AActor* NewTargetActorRef);

	UFUNCTION(BlueprintCallable, Meta = (HideSelfPin = "true"))
	void CombatCheck();

	UFUNCTION(BlueprintCallable, Meta = (HideSelfPin = "true"))
	void RaiseResetAttackCombo();

	UFUNCTION(BlueprintCallable, Meta = (HideSelfPin = "true"))
	void RaiseResetAttack();
};
