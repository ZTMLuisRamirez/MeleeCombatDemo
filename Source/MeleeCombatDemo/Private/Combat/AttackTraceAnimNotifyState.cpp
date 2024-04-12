// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/AttackTraceAnimNotifyState.h"
#include "Combat/TraceComponent.h"

void UAttackTraceAnimNotifyState::NotifyBegin(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	float TotalDuration,
	const FAnimNotifyEventReference &EventReference
)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	ToggleAttack(MeshComp, true);
}

void UAttackTraceAnimNotifyState::NotifyEnd(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference &EventReference
)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	ToggleAttack(MeshComp, false);
}

void UAttackTraceAnimNotifyState::ToggleAttack(USkeletalMeshComponent* MeshComp, bool NewVal)
{
	AActor* OwnerRef = MeshComp->GetOwner(); 
	UTraceComponent* TraceComp = OwnerRef->FindComponentByClass<UTraceComponent>();

	if (!IsValid(TraceComp)) 
	{ 
		UE_LOG(
			LogClass,
			Warning,
			TEXT("%s does not have the trace component. Message from the Attack Trace Notify State."),
			*OwnerRef->GetName()
		)
		return; 
	}

	TraceComp->bIsAttacking = NewVal;
}
