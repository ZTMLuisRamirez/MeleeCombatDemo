// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/EnemyCharacter.h"
#include "Characters/StatsComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/WidgetComponent.h"
#include "Characters/MainCharacter.h"
#include "Interfaces/Combat.h"
#include "Characters/PatrolComponent.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/EEnemyState.h"
#include "Combat/AttackComponent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StatsComp = CreateDefaultSubobject<UStatsComponent>(TEXT("StatsComponent"));
	PatrolComp = CreateDefaultSubobject<UPatrolComponent>(TEXT("PatrolComponent"));
	AttackComp = CreateDefaultSubobject<UAttackComponent>(TEXT("AttackComponent"));
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	LockonWidget = FindComponentByClass<UWidgetComponent>();

	GetController<AAIController>()->GetBlackboardComponent()->SetValueAsEnum(
		TEXT("CurrentState"),
		InitialState
	);
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacter::ReceiveDamage(float Damage)
{
	//if (bIsDead) { return; }

	//GetController<AAIController>()->GetBlackboardComponent()->SetValueAsEnum(
	//	TEXT("CurrentState"),
	//	EEnemyCurrentState::Hit
	//);

	StatsComp->Stats[StatType::Health] -= Damage;

	if (StatsComp->Stats[StatType::Health] > 0)
	{
		float Duration = PlayAnimMontage(HitAnimation);

		//GetWorldTimerManager().SetTimer(
		//	AttackTimerHandle,
		//	this,
		//	&AMinionCharacter::FinishHitAnim,
		//	Duration,
		//	false
		//);

		return;
	}

	//GetController<AAIController>()->GetBrainComponent()->StopLogic("die");
	//GetController<AAIController>()->ClearFocus(EAIFocusPriority::Gameplay);
	bIsDead = true;
	PlayAnimMontage(DeathAnimation);

	ACharacter* CharacterRef{
		GetWorld()->GetFirstPlayerController()->GetCharacter()
	};

	if (!CharacterRef) { return; } 
	
	if (!CharacterRef->Implements<UCombat>()) { return; }

	ICombat* CombatRef { Cast<ICombat>(CharacterRef) };

	if (!CombatRef) { return; }

	CombatRef->EndLockonWithActor(this);
}

void AEnemyCharacter::OnSelect()
{
	LockonWidget->SetVisibility(true);
}

void AEnemyCharacter::OnDeselect()
{
	LockonWidget->SetVisibility(false);
}

bool AEnemyCharacter::IsDead()
{
	return bIsDead;
}

void AEnemyCharacter::Attack()
{
	AttackComp->RandomAttack();
}

float AEnemyCharacter::GetAnimDuration()
{
	return AttackComp->AnimDuration;
}
