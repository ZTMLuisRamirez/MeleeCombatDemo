// Fill out your copyright notice in the Description page of Project Settings.


#include "Animations/RotateAnimNotifyState.h"
#include "Characters/RotationComponent.h"

void URotateAnimNotifyState::NotifyBegin(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	float TotalDuration,
	const FAnimNotifyEventReference& EventReference
)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	AActor* ActorRef = MeshComp->GetOwner();

	if (!IsValid(ActorRef)) { return; }

	URotationComponent* RotationComp = ActorRef->FindComponentByClass<URotationComponent>();

	if (!IsValid(RotationComp)) { return; }

	RotationComp->bCanRotate = true;
}

void URotateAnimNotifyState::NotifyEnd(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference
)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	AActor* ActorRef = MeshComp->GetOwner(); 

	if (!IsValid(ActorRef)) { return; } 

	URotationComponent* RotationComp = ActorRef->FindComponentByClass<URotationComponent>();

	if (!IsValid(RotationComp)) { return; } 

	RotationComp->bCanRotate = false;
}