// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BossCharacter.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Combat/EnemyProjectileComponent.h"
#include "Combat/TraceComponent.h"
#include "Combat/AttackComponent.h"
#include "Characters/StatsComponent.h"
#include "Characters/RotationComponent.h"
#include "BrainComponent.h"
#include "Components/CapsuleComponent.h"
#include "Blueprint/UserWidget.h"
#include "UI/BossWidget.h"

// Sets default values
ABossCharacter::ABossCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TraceComp = CreateDefaultSubobject<UTraceComponent>(TEXT("TraceComponent"));
	EnemyProjectileComp = CreateDefaultSubobject<UEnemyProjectileComponent>(TEXT("EnemyProjectileComponent"));
	AttackComp = CreateDefaultSubobject<UAttackComponent>(TEXT("AttackComponent"));
	StatsComp = CreateDefaultSubobject<UStatsComponent>(TEXT("StatsComponent"));
	RotationComp = CreateDefaultSubobject<URotationComponent>(TEXT("RotationComponent"));
}

// Called when the game starts or when spawned
void ABossCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	GetController<AAIController>()->GetBlackboardComponent()->SetValueAsEnum(
		TEXT("CurrentState"),
		InitialState
	);

	WidgetInstance = Cast<UBossWidget>(UUserWidget::CreateWidgetInstance(
		*GetWorld(), WidgetTemplate, "Boss HUD"
	));
}

// Called every frame
void ABossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABossCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABossCharacter::Attack()
{
	AttackComp->RandomAttack();
}

float ABossCharacter::GetAnimDuration()
{
	return AttackComp->AnimDuration;
}

float ABossCharacter::GetDamage()
{
	return StatsComp->Stats[StatType::Strength];
}

void ABossCharacter::ReceiveDamage(float Damage)
{
	if (StatsComp->Stats[StatType::Health] <= 0) { return; }

	StatsComp->Stats[StatType::Health] -= Damage;

	StatsComp->BroadcastHealthUpdate();

	if (StatsComp->Stats[StatType::Health] > 0) { return; }

	GetController<AAIController>()->GetBrainComponent()->StopLogic("defeated");
	GetController<AAIController>()->ClearFocus(EAIFocusPriority::Gameplay);

	FindComponentByClass<UCapsuleComponent>()->SetCollisionEnabled(
		ECollisionEnabled::NoCollision
	); 

	PlayAnimMontage(DeathAnimation);

	ACharacter* CharacterRef{
		GetWorld()->GetFirstPlayerController()->GetCharacter()
	};

	if (!CharacterRef) { return; }

	if (!CharacterRef->Implements<UCombat>()) { return; }

	ICombat* CombatRef{ Cast<ICombat>(CharacterRef) };

	if (!CombatRef) { return; }

	CombatRef->EndLockonWithActor(this);
}

void ABossCharacter::DetectPawn(APawn* DetectedPawn, APawn* PawnToDetect)
{

	UBlackboardComponent* BlackboardComp{
		GetController<AAIController>()->GetBlackboardComponent()
	};

	EEnemyState CurrentState{ static_cast<EEnemyState>(
		BlackboardComp->GetValueAsEnum(TEXT("CurrentState"))
	) };

	if (
		DetectedPawn != PawnToDetect || CurrentState == EEnemyState::Combat
	) {
		return;
	}

	BlackboardComp->SetValueAsEnum(
		TEXT("CurrentState"),
		EEnemyState::Combat
	);

	if (WidgetInstance->IsInViewport()) { return; }

	WidgetInstance->AddToViewport(10);

	StatsComp->BroadcastHealthUpdate();
}