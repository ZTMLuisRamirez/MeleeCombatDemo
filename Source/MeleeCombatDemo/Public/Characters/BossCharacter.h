// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/EEnemyState.h"
#include "Interfaces/Enemy.h"
#include "Interfaces/Combat.h"
#include "BossCharacter.generated.h"

UCLASS()
class MELEECOMBATDEMO_API ABossCharacter : public ACharacter, public IEnemy, public ICombat
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (MetaClass = "UserWidget"))
	TSubclassOf<class UBossWidget> WidgetTemplate;

	class UWidgetComponent* LockonWidget;

public:
	// Sets default values for this character's properties
	ABossCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTraceComponent* TraceComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UEnemyProjectileComponent* EnemyProjectileComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	class UAttackComponent* AttackComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStatsComponent* StatsComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class URotationComponent* RotationComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText BossDisplayName;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EEnemyState> InitialState;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DeathAnimation;

	UPROPERTY(BlueprintReadWrite)
	class UBossWidget* WidgetInstance;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Attack() override;

	virtual float GetAnimDuration() override;

	virtual float GetDamage() override;

	UFUNCTION(BlueprintCallable, Meta = (HideSelfPin = "true"))
	void ReceiveDamage(float Damage); 

	UFUNCTION(BlueprintCallable, Meta = (HideSelfPin = "true"))
	void DetectPawn(APawn* DetectedPawn, APawn* PawnToDetect);

	virtual void OnSelect() override;

	virtual void OnDeselect() override;

	virtual bool IsDead() override;

	UFUNCTION()
	void HandlePlayerZeroHealth();

	UFUNCTION()
	void HandleDeath();
};
 