// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BossAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MELEECOMBATDEMO_API UBossAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	APawn* ActorRef;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float VerticalSpeed{ 0.f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UEnemyProjectileComponent* ProjectileComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTraceComponent* TraceComp;

	virtual void NativeBeginPlay() override;

	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsCharging{ false };

	bool bIsChargingAndMoving{ false };

	UFUNCTION(BlueprintCallable)
	void HandleTravelFwdStateEntry();
};
