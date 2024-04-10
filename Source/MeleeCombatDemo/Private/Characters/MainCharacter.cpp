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

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	LockonComp = CreateDefaultSubobject<ULockonComponent>(TEXT("LockonComponent"));
	AttackComp = CreateDefaultSubobject<UAttackComponent>(TEXT("AttackComponent"));
	TraceComp = CreateDefaultSubobject<UTraceComponent>(TEXT("TraceComponent")); 
	StatsComp = CreateDefaultSubobject<UStatsComponent>(TEXT("StatsComponent"));
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerAnim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());

	if (PlayerAnim != nullptr)
	{
		PlayerAnim->OnResetAttackComboDelegate.AddDynamic(
			AttackComp, &UAttackComponent::HandleResetAttackCombo
		);

		PlayerAnim->OnResetAttackDelegate.AddDynamic(
			AttackComp, &UAttackComponent::HandleResetAttack
		);

		PlayerAnim->OnResetAttackDelegate.AddDynamic(
			TraceComp, &UTraceComponent::HandleResetAttack
		);
	}
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

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

void AMainCharacter::Sprint()
{
	if (!HasEnoughStamina(1.0f))
	{
		Walk();
		return;
	}

	if (GetCharacterMovement()->Velocity.Length() < 1) { return; }

	GetCharacterMovement()->MaxWalkSpeed = StatsComp->Stats[StatType::SprintSpeed];
	StatsComp->Stats[StatType::Stamina] -= 0.1f;
	StatsComp->bCanRegen = false;
}

void AMainCharacter::Walk()
{
	GetCharacterMovement()->MaxWalkSpeed = StatsComp->Stats[StatType::WalkSpeed];

	StatsComp->DelayStaminaRegen(); 
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

void AMainCharacter::ReceiveDamage(float Damage)
{
	if (bIsDead) { return; }

	StatsComp->Stats[StatType::Health] -= Damage;

	PlayerWidget->AdjustHealth(
		StatsComp->Stats[StatType::Health], StatsComp->Stats[StatType::MaxHealth]
	);

	if (StatsComp->Stats[StatType::Health] > 0)
	{
		//AttackComp->HandleResetAttack();

		float Duration = PlayAnimMontage(HitAnimation);

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
