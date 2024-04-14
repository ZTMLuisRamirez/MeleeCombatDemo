// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/BlockComponent.h"
#include "GameFramework/Character.h"

void UBlockComponent::BeginPlay()
{
	OwnerRef = GetOwner<ACharacter>();

	if (OwnerRef->Implements<UCombat>())
	{
		IFighterRef = Cast<ICombat>(OwnerRef);
	}

	if (OwnerRef->Implements<UMainPlayer>())
	{
		IPlayerRef = Cast<IMainPlayer>(OwnerRef);
	}
}

bool UBlockComponent::Check(AActor* Opponent)
{
	FVector DamageCauserLocation{ Opponent->GetActorForwardVector() };

	// Positive if looking away. Negative if looking at each other.
	double result{
		FVector::DotProduct(
			DamageCauserLocation, OwnerRef->GetActorForwardVector()
		)
	};

	// Block attack if player is blocking
	if (
		result > 0 ||
		!IFighterRef->IsPlayingBlockAnimation() ||
		!IPlayerRef->HasEnoughStamina(StaminaCost)
	)
	{
		return false;
	}

	// Reduce Stamina & Delay the stamina regen
	OnBlockDelegate.Broadcast(StaminaCost);

	OwnerRef->PlayAnimMontage(ImpactAnimMontage);

	return true;
}