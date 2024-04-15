// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MainCharacter.h"
#include "Combat/LockonComponent.h"
#include "Combat/AttackComponent.h"
#include "Combat/TraceComponent.h"
#include "Characters/StatsComponent.h"
#include "Combat/BlockComponent.h"
#include "Animations/PlayerAnimInstance.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/PlayerUserWidget.h"
#include "Characters/PlayerActionsComponent.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	LockonComp = CreateDefaultSubobject<ULockonComponent>(TEXT("LockonComponent"));
	AttackComp = CreateDefaultSubobject<UAttackComponent>(TEXT("AttackComponent"));
	TraceComp = CreateDefaultSubobject<UTraceComponent>(TEXT("TraceComponent"));
	StatsComp = CreateDefaultSubobject<UStatsComponent>(TEXT("StatsComponent"));
	PlayerActionsComp = CreateDefaultSubobject<UPlayerActionsComponent>(TEXT("PlayerActionsComponent"));
	BlockComp = CreateDefaultSubobject<UBlockComponent>(TEXT("BlockComponent"));
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerAnim = Cast<UPlayerAnimInstance>(
		GetMesh()->GetAnimInstance()
	);
}

void AMainCharacter::LoadPlayerHUD()
{
	UUserWidget* WidgetInstance = UUserWidget::CreateWidgetInstance(
		*GetWorld(), PlayerWidgetTemplate, "Player HUD"
	);
	WidgetInstance->AddToViewport(10);

	PlayerWidget = Cast<UPlayerUserWidget>(WidgetInstance);

	PlayerWidget->AdjustHealth( 
		StatsComp->Stats[StatType::Health], StatsComp->Stats[StatType::MaxHealth]
	);
}

bool AMainCharacter::HasEnoughStamina(float RequiredCost)
{
	return StatsComp->Stats[StatType::Stamina] >= RequiredCost;
}

float AMainCharacter::GetDamage()
{
	return StatsComp->Stats[StatType::Strength];
}

void AMainCharacter::EndLockonWithActor(AActor* ActorRef)
{
	if (LockonComp->CurrentTargetActor != ActorRef) { return; }

	LockonComp->EndLockon();
}

void AMainCharacter::HandleDeath()
{
	float Duration = PlayAnimMontage(DeathAnimMontage);

	DisableInput(GetController<APlayerController>());

	FTimerHandle DeathTimerHandler;
	GetWorldTimerManager().SetTimer(
		DeathTimerHandler,
		this,
		&AMainCharacter::LoadWidget,
		Duration,
		false
	);
}

void AMainCharacter::LoadWidget()
{
	UUserWidget* NewWidget = UUserWidget::CreateWidgetInstance(
		*GetWorld(), DeathWidgetTemplate, "Death Widget"
	);
	NewWidget->AddToViewport(20);
}

void AMainCharacter::ToggleBlock(bool bBlockFlag)
{
	PlayerAnim->bIsBlocking = bBlockFlag;
}

bool AMainCharacter::IsDead()
{
	return StatsComp->Stats[StatType::Health] <= 0;
}

bool AMainCharacter::IsBlocking(AActor* Opponent)
{
	return BlockComp->Check(Opponent);
}

bool AMainCharacter::IsPlayingBlockAnimation()
{
	return PlayerAnim->bIsBlocking;
}

float AMainCharacter::GetCharacterHealth()
{
	return StatsComp->Stats[StatType::Health]; 
}

bool AMainCharacter::IsRolling()
{
	return PlayerActionsComp->bIsRollActive;
}