// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/EnemyCharacter.h"
#include "Characters/StatsComponent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StatsComp = CreateDefaultSubobject<UStatsComponent>(TEXT("StatsComponent"));
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
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
	UE_LOG(LogClass, Warning, TEXT("Death Animation"));
	bIsDead = true;
	PlayAnimMontage(DeathAnimation);

	//ACharacter* CharacterRef = GetWorld()->GetFirstPlayerController()
	//	->GetCharacter();
	//AMyProjectCharacter* PlayerRef = Cast<AMyProjectCharacter>(CharacterRef);

	//if (!IsValid(PlayerRef->TargetActor)) { return; }

	//if (PlayerRef->TargetActor != this) { return; }

	//PlayerRef->EndLockon();
}