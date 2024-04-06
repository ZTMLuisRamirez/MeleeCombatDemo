// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

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

public:
	UFUNCTION(BlueprintCallable, Meta = (HideSelfPin = "true"))
	void UpdateSpeedWithVelocity();

	UFUNCTION(BlueprintCallable, Meta = (HideSelfPin = "true"))
	void UpdateFalling();
};
