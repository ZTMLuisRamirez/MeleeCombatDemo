// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MainCharacter.h"
#include "Combat/LockonComponent.h"
#include "Combat/AttackComponent.h"
#include "Animations/PlayerAnimInstance.h"
#include "Combat/TraceComponent.h"
#include "Characters/StatsComponent.h"
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
}

UPlayerAnimInstance* AMainCharacter::GetPlayerAnimInstance() const
{
	return Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());;
}

void AMainCharacter::ToggleTrace(bool bIsTracing)
{
	TraceComp->bIsAttacking = bIsTracing;
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

void AMainCharacter::ReceiveDamage(float Damage, AActor* DamageCauser)
{
	if (bIsDead) { return; }

	FVector DamageCauserLocation{ DamageCauser->GetActorForwardVector()};

	double result{
		FVector::DotProduct(DamageCauserLocation, GetActorForwardVector())
	};

	// Block attack if player is blocking
	if (result <= 0 && PlayerAnim->bIsBlocking)
	{
		if (HasEnoughStamina(AttackComp->BlockStaminaCost))
		{
			AttackComp->BroadcastBlockDelegate();

			PlayAnimMontage(BlockAnimation);

			StatsComp->DelayStaminaRegen();

			//PlayerWidget->UpdateStaminaProgressBar(StatType::Stamina, 10.0f);

			return;
		}
	}

	StatsComp->Stats[StatType::Health] -= Damage;

	PlayerWidget->AdjustHealth(
		StatsComp->Stats[StatType::Health], StatsComp->Stats[StatType::MaxHealth]
	);

	if (StatsComp->Stats[StatType::Health] > 0)
	{
		AttackComp->HandleResetAttack();
		StatsComp->HandleAttackComplete();

		float Duration = PlayAnimMontage(HitAnimation);

		// Perform Camera Shake
		GetController<APlayerController>()->ClientStartCameraShake(
			CameraShakeTemplate
		);

	//	GetWorldTimerManager().SetTimer(
	//		AttackTimerHandle,
	//		this,
	//		&AEnemyCharacter::FinishHitAnim,
	//		Duration,
	//		false
	//	);

		return;
	}

	bIsDead = true;

	float Duration = PlayAnimMontage(DeathAnimation);

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
