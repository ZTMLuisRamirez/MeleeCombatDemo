// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/Combat.h"
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
	FOnHitSignature,
	UAttackComponent, OnHitDelegate,
	float, Amount
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

	ACharacter* OwnerRef;

	ICombat* IFighterRef;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UCameraShakeBase> CameraShakeTemplate;
public:	
	UPROPERTY(BlueprintAssignable)
	FOnAttackPerformedSignature OnAttackPerformedDelegate;

	UPROPERTY(BlueprintAssignable)
	FOnAttackCompleteSignature OnAttackCompleteDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AnimDuration;

	UPROPERTY(BlueprintAssignable)
	FOnHitSignature OnHitDelegate;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UFUNCTION(BlueprintCallable)
	void ComboAttack();

	UFUNCTION(BlueprintCallable)
	void HandleResetAttackCombo();

	UFUNCTION(BlueprintCallable)
	void HandleResetAttack();

	void RandomAttack();

	UFUNCTION(BlueprintCallable)
	void ReceiveDamage(float Damage, AActor* DamageCauser);
};
