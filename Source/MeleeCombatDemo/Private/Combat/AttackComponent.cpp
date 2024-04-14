// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/AttackComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Interfaces/MainPlayer.h"


// Called when the game starts
void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerRef = Cast<ACharacter>(GetOwner());

	if (!OwnerRef->Implements<UCombat>()) { return; }

	IFighterRef = Cast<ICombat>(OwnerRef);
}

void UAttackComponent::ComboAttack()
{
	if (OwnerRef->Implements<UMainPlayer>())
	{ 
		IMainPlayer* IPlayerRef = Cast<IMainPlayer>(OwnerRef);

		if (IPlayerRef && !IPlayerRef->HasEnoughStamina(StaminaCost)) { return; }
	}

	if (!bCanAttack) { return; }

	bCanAttack = false;

	int32 MaxCombo = AttackAnimations.Num();

	OwnerRef->PlayAnimMontage(AttackAnimations[ComboCounter]);
	
	ComboCounter++;

	ComboCounter = UKismetMathLibrary::Wrap(
		ComboCounter, -1, (MaxCombo - 1)
	);

	OnAttackPerformedDelegate.Broadcast(StaminaCost);
}

void UAttackComponent::HandleResetAttackCombo()
{
	ComboCounter = 0;
	OnAttackCompleteDelegate.Broadcast();
}

void UAttackComponent::HandleResetAttack()
{
	bCanAttack = true;
}

void UAttackComponent::RandomAttack()
{
	/*if (!CanAttack || CurrentStamina < AttackStaminaCost) { return; }

	CanAttack = false;*/

	int RandomIndex = FMath::RandRange(0, AttackAnimations.Num() - 1);

	AnimDuration = OwnerRef->PlayAnimMontage(AttackAnimations[RandomIndex]);
}

void UAttackComponent::ReceiveDamage(float Damage, AActor* DamageCauser)
{
	if (IFighterRef->IsDead()) { return; }

	// Check if player to determine if they can block
	if (IFighterRef->IsBlocking(DamageCauser)) { return; }

	// Character Damaged
	OnHitDelegate.Broadcast(Damage);

	// Check if character didn't die!
	if (IFighterRef->GetCharacterHealth() > 0)
	{
		if (OwnerRef->IsPlayerControlled())
		{
			HandleResetAttack();

			OwnerRef->PlayAnimMontage(HitAnimMontage);

			// Perform Camera Shake
			OwnerRef->GetController<APlayerController>()
				->ClientStartCameraShake(
					CameraShakeTemplate
				);
		}
	}
}
