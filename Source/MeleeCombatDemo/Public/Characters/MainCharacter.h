// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/Combat.h"
#include "Interfaces/MainPlayer.h"
#include "MainCharacter.generated.h"

UCLASS()
class MELEECOMBATDEMO_API AMainCharacter : public ACharacter, public IMainPlayer, public ICombat
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (MetaClass = "UserWidget"))
	TSubclassOf<UUserWidget> PlayerWidgetTemplate;

	UPROPERTY(EditAnywhere, meta = (MetaClass = "UserWidget"))
	TSubclassOf<UUserWidget> DeathWidgetTemplate;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBlockComponent* BlockComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPlayerActionsComponent* PlayerActionsComp;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UPlayerAnimInstance* PlayerAnim;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UPlayerUserWidget* PlayerWidget;

	UPROPERTY(EditAnywhere)
	UAnimMontage* DeathAnimMontage;

public:	
	UFUNCTION(BlueprintCallable)
	void LoadPlayerHUD();

	virtual bool HasEnoughStamina(float RequiredCost) override;

	virtual float GetDamage() override;

	virtual void EndLockonWithActor(AActor* ActorRef) override;

	UFUNCTION()
	void LoadWidget();

	UFUNCTION(BlueprintCallable)
	void ToggleBlock(bool bBlockFlag);

	virtual bool IsDead() override;

	virtual bool IsBlocking(AActor* Opponent) override;

	virtual bool IsPlayingBlockAnimation() override;

	virtual float GetCharacterHealth() override;

	UFUNCTION(BlueprintCallable)
	void HandleDeath();
};
