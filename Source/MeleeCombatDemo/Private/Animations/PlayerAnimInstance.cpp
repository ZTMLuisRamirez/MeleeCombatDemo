// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/PlayerAnimInstance.h"
#include "GameFramework/PawnMovementComponent.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	PawnRef = TryGetPawnOwner();
}

void UPlayerAnimInstance::UpdateSpeedWithVelocity()
{
	if (!IsValid(PawnRef)) { return; }

	FVector Velocity = PawnRef->GetVelocity();
	VerticalSpeed = static_cast<float>(Velocity.Length());
}

void UPlayerAnimInstance::UpdateFalling()
{
	if (!IsValid(PawnRef)) { return; }

	UPawnMovementComponent* MovementComponent = PawnRef->GetMovementComponent();
	bIsFalling = MovementComponent->IsFalling();
}

void UPlayerAnimInstance::HandleUpdatedTarget(AActor* NewTargetActorRef)
{
	UE_LOG(LogClass, Warning, TEXT("UPDATED TARGET"));
	bIsInCombat = IsValid(NewTargetActorRef);
}

void UPlayerAnimInstance::CombatCheck()
{
	if (!IsValid(PawnRef)) { return; }

	if (!bIsInCombat) { return; }

	Direction = CalculateDirection(
		PawnRef->GetVelocity(),
		PawnRef->GetActorRotation()
	);
}