// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatsComponent.generated.h"

UENUM(BlueprintType)
enum StatType
{
	Health UMETA(DisplayName = "Health"),
	MaxHealth UMETA(DisplayName = "Max Health"),
	Strength UMETA(DisplayName = "Strength"),
	Stamina UMETA(DisplayName = "Stamina"),
	MaxStamina UMETA(DisplayName = "Max Stamina"),
	WalkSpeed UMETA(DisplayName = "Walk Speed"),
	SprintSpeed UMETA(DisplayName = "Sprint Speed")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnUpdateStatSignature, StatType, TargetStat, float, NewVal
);

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_TwoParams(
	FOnHealthUpdateSignature, 
	UStatsComponent, OnHealthUpdateDelegate,
	float, CurrentHealthVal, 
	float, MaxHealthVal
);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MELEECOMBATDEMO_API UStatsComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float StaminaRegenRate{ 10.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float StaminaDelayTime{ 2.0f };
public:	
	// Sets default values for this component's properties
	UStatsComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bCanRegen{ true };

	UPROPERTY(BlueprintAssignable)
	FOnHealthUpdateSignature OnHealthUpdateDelegate;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	TMap<TEnumAsByte<StatType>, float> Stats;

	UFUNCTION(BlueprintCallable)
	void RegenStamina();

	UPROPERTY(BlueprintAssignable)
	FOnUpdateStatSignature OnUpdateStatDelegate;

	UFUNCTION(BlueprintCallable)
	void HandleAttackPerformed(float Amount);

	UFUNCTION(BlueprintCallable)
	void HandleAttackComplete();

	void DelayStaminaRegen();

	UFUNCTION()
	void EnableRegen();

	UFUNCTION(BlueprintCallable)
	void HandleBlock(float Amount);

	UFUNCTION(BlueprintCallable)
	void HandleRoll(float Amount);

	void BroadcastHealthUpdate();
};
