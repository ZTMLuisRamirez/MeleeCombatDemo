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
	MaxStamina UMETA(DisplayName = "Max Stamina")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FOnUpdateStatSignature, StatType, TargetStat, float, NewVal
);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MELEECOMBATDEMO_API UStatsComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float StaminaRegenRate{ 10.0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bCanRegen{ true };
public:	
	// Sets default values for this component's properties
	UStatsComponent();

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

	UFUNCTION()
	void HandleAttackPerformed(float Amount);

	UFUNCTION()
	void HandleAttackComplete();
};
