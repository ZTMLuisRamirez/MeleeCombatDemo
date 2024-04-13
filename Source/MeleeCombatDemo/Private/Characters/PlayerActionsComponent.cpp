// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerActionsComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

// Called when the game starts
void UPlayerActionsComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerRef = GetOwner<ACharacter>();
	
	if (!OwnerRef->Implements<UMainPlayer>()) { return; }

	IPlayerRef = Cast<IMainPlayer>(OwnerRef);
}

void UPlayerActionsComponent::Sprint()
{
	if (!IPlayerRef->HasEnoughStamina(MinSprintStaminaRequired))
	{
		Walk();
		return;
	}

	if (OwnerRef->GetCharacterMovement()->Velocity.Length() < 1) { return; }

	OwnerRef->GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;

	OnSprintDelegate.Broadcast(SprintCost);
}

void UPlayerActionsComponent::Walk()
{
	OwnerRef->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	OnWalkDelegate.Broadcast();
}