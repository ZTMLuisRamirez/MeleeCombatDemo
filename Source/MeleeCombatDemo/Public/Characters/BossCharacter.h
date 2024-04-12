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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EEnemyState> InitialState;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Attack() override;

	virtual float GetAnimDuration() override;

	virtual float GetDamage() override;

	/*virtual class AEnemyProjectile* GetProjectileComponent() override;*/
};
