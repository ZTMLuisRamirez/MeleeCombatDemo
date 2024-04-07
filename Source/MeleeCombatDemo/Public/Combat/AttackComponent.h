// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttackComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnAttackPerformedSignature, float, Amount
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackCompleteSignature);


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
public:	
	// Sets default values for this component's properties
	UAttackComponent();

	FOnAttackPerformedSignature OnAttackPerformedDelegate;

	FOnAttackCompleteSignature OnAttackCompleteDelegate;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void ComboAttack();

	UFUNCTION()
	void HandleResetAttackCombo();

	UFUNCTION()
	void HandleResetAttack();
};
