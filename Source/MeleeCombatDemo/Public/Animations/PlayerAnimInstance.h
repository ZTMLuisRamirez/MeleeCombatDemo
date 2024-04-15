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
	virtual void NativeBeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VerticalSpeed{ 0.f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsFalling{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Direction{ 0 };

	UPROPERTY(BlueprintReadWrite);
	class AMainCharacter* MainCharacterRef;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsInCombat{ false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsBlocking{ false };

	UFUNCTION(BlueprintCallable, Meta = (HideSelfPin = "true"))
	void UpdateSpeedWithVelocity();

	UFUNCTION(BlueprintCallable, Meta = (HideSelfPin = "true"))
	void UpdateFalling();

	UFUNCTION(BlueprintCallable)
	void HandleUpdatedTarget(AActor* NewTargetActorRef);

	UFUNCTION(BlueprintCallable, Meta = (HideSelfPin = "true"))
	void CombatCheck();
};
