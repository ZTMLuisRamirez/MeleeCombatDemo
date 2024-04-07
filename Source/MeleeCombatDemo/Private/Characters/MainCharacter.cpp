// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MainCharacter.h"
#include "Combat/LockonComponent.h"
#include "Combat/AttackComponent.h"
#include "Animations/PlayerAnimInstance.h"
#include "Combat/TraceComponent.h"
#include "Characters/StatsComponent.h"
#include "Blueprint/UserWidget.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	LockonComp = FindComponentByClass<ULockonComponent>();
	PlayerAnim = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	AttackComp = FindComponentByClass<UAttackComponent>();
	TraceComp = FindComponentByClass<UTraceComponent>(); 
	StatsComp = FindComponentByClass<UStatsComponent>();

	if (PlayerAnim != nullptr)
	{
		/*FScriptDelegate ScriptDelegate;
		ScriptDelegate.BindUFunction(PlayerAnim, "HandleUpdatedTarget"); */
		LockonComp->OnUpdatedTargetDelegate.AddDynamic(
			PlayerAnim, &UPlayerAnimInstance::HandleUpdatedTarget
		);
	}

	if (AttackComp != nullptr)
	{
		PlayerAnim->OnResetAttackComboDelegate.AddDynamic(
			AttackComp, &UAttackComponent::HandleResetAttackCombo
		);

		PlayerAnim->OnResetAttackDelegate.AddDynamic(
			AttackComp, &UAttackComponent::HandleResetAttack
		);

		AttackComp->OnAttackPerformedDelegate.AddDynamic(
			StatsComp, &UStatsComponent::HandleAttackPerformed
		);

		AttackComp->OnAttackCompleteDelegate.AddDynamic(
			StatsComp, &UStatsComponent::HandleAttackComplete
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
	//UUserWidget* NewWidget = CreateWidget(GetWorld(), DeathWidget);
	//NewWidget->AddToViewport(9999); // Z-order, this just makes it render on the very top./
}

bool AMainCharacter::HasEnoughStamina(float RequiredCost)
{
	if (StatsComp == nullptr) { return true; }

	return StatsComp->Stats[StatType::Stamina] >= RequiredCost;
}

