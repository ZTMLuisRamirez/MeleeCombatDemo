// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/AttackTracing.h"
#include "Interfaces/Stamina.h"
#include "MainCharacter.generated.h"

UCLASS()
class MELEECOMBATDEMO_API AMainCharacter : public ACharacter, public IAttackTracing, public IStamina
{
	GENERATED_BODY()

	class UPlayerAnimInstance* PlayerAnim; 

	UPROPERTY(EditAnywhere, meta = (MetaClass = "UserWidget"))
	TSubclassOf<UUserWidget> PlayerWidgetTemplate;

public:
	// Sets default values for this character's properties
	AMainCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class ULockonComponent* LockonComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UAttackComponent* AttackComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UTraceComponent* TraceComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStatsComponent* StatsComp;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	class UPlayerAnimInstance* GetPlayerAnimInstance() const;

	virtual void ToggleTrace(bool bIsTracing) override;

	UFUNCTION(BlueprintCallable)
	void LoadPlayerHUD();

	virtual bool HasEnoughStamina(float RequiredCost) override;

	UFUNCTION(BlueprintCallable, Meta = (HideSelfPin = "true"))
	void Sprint();

	UFUNCTION(BlueprintCallable, Meta = (HideSelfPin = "true"))
	void Walk();
};
