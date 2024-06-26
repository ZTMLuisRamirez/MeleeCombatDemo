// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/MainPlayer.h"
#include "PlayerActionsComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(
	FOnSprintSignature, 
	UPlayerActionsComponent, OnSprintDelegate,
	float, Cost
);

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(
	FOnWalkSignature,
	UPlayerActionsComponent, OnWalkDelegate
);

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(
	FOnRollSignature,
	UPlayerActionsComponent, OnRollDelegate,
	float, Cost
);

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(
	FOnRollCompleteSignature,
	UPlayerActionsComponent, OnRollCompleteDelegate
);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MELEECOMBATDEMO_API UPlayerActionsComponent : public UActorComponent
{
	GENERATED_BODY()
	
	ACharacter* OwnerRef;

	IMainPlayer* IPlayerRef;

public:
	UPROPERTY(BlueprintAssignable)
	FOnSprintSignature OnSprintDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnWalkSignature OnWalkDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnRollSignature OnRollDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* RollAnimation;

	UPROPERTY(BlueprintAssignable)
	FOnRollCompleteSignature OnRollCompleteDelegate;

	UPROPERTY(VisibleAnywhere)
	bool bIsRollActive{ false };

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinSprintStaminaRequired{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SprintCost{ 0.1f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SprintSpeed{ 1000.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WalkSpeed{ 600.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RollCost{ 5.0f };

public:	
	UFUNCTION(BlueprintCallable)
	void Sprint();

	UFUNCTION(BlueprintCallable)
	void Walk();

	UFUNCTION(BlueprintCallable)
	void Roll();

	UFUNCTION()
	void FinishRollAnim();
};
