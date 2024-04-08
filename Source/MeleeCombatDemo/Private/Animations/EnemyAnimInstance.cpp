// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/EnemyAnimInstance.h"

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	ActorRef = TryGetPawnOwner();
}

void UEnemyAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (!IsValid(ActorRef)) { return; }

	FVector Velocity = ActorRef->GetVelocity();
	VerticalSpeed = static_cast<float>(Velocity.Length());
}