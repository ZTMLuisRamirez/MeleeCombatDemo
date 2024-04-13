// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttackComponent.generated.h"

class UAttackComponent;

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(
	FOnAttackPerformedSignature, 
	UAttackComponent, OnAttackPerformedDelegate,
	float, Amount
);

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(
	FOnAttackCompleteSignature,
	UAttackComponent, OnAttackCompleteDelegate
);

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(
	FOnBlockSignature,
	UAttackComponent, OnBlockDelegate,
	float, Cost
);

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(
	FOnRollSignature,
	UAttackComponent, OnRollDelegate,
	float, Cost
);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MELEECOMBATDEMO_API UAttackComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> AttackAnimations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	int32 ComboCounter{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bCanAttack{ true };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float StaminaCost{ 15.0f };

	ACharacter* OwnerCharacter;
public:	
	// Sets default values for this component's properties
	UAttackComponent();

	UPROPERTY(BlueprintAssignable)
	FOnAttackPerformedSignature OnAttackPerformedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnAttackCompleteSignature OnAttackCompleteDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AnimDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float BlockStaminaCost{ 10.0f };

	UPROPERTY(BlueprintAssignable)
	FOnBlockSignature OnBlockDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float RollStaminaCost{ 5.0f };

	UPROPERTY(BlueprintAssignable)
	FOnRollSignature OnRollDelegate;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void ComboAttack();

	UFUNCTION(BlueprintCallable)
	void HandleResetAttackCombo();

	UFUNCTION(BlueprintCallable)
	void HandleResetAttack();

	void RandomAttack();

	void BroadcastBlockDelegate();

	void BroadcastRollDelegate();
};
