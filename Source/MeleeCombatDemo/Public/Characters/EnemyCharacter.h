// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/TargetableInterface.h"
#include "Interfaces/Combat.h"
#include "Interfaces/AttackTracing.h"
#include "EnemyCharacter.generated.h"


UCLASS()
class MELEECOMBATDEMO_API AEnemyCharacter : public ACharacter, public ITargetableInterface, public ICombat, public IAttackTracing
{
	GENERATED_BODY()

	class UWidgetComponent* LockonWidget;

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStatsComponent* StatsComp;

	bool bIsDead{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HitAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DeathAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPatrolComponent* PatrolComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<enum EEnemyState> InitialState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAttackComponent* AttackComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTraceComponent* TraceComp;

	FTimerHandle AttackTimerHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Meta = (HideSelfPin = "true"))
	void ReceiveDamage(float Damage);

	virtual void OnSelect() override;

	virtual void OnDeselect() override;

	virtual bool IsDead() override;

	virtual void Attack() override;

	virtual float GetAnimDuration() override;

	virtual void ToggleTrace(bool bIsTracing) override;

	UFUNCTION(BlueprintCallable, Meta = (HideSelfPin = "true"))
	void DetectPawn(APawn* DetectedPawn, APawn* PawnToDetect);

	UFUNCTION()
	void FinishHitAnim();

	UFUNCTION(BlueprintCallable, Meta = (HideSelfPin = "true"))
	void LoseSightOfPlayer(AActor* OtherActor);
};
