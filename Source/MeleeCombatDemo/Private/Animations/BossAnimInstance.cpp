// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/BossAnimInstance.h"

void UBossAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	ActorRef = TryGetPawnOwner();
}

void UBossAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (!IsValid(ActorRef)) { return; }

	FVector Velocity = ActorRef->GetVelocity();
	VerticalSpeed = static_cast<float>(Velocity.Length());
}
