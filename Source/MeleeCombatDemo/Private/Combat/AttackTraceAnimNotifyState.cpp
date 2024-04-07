// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/AttackTraceAnimNotifyState.h"
#include "Interfaces/AttackTracing.h"

void UAttackTraceAnimNotifyState::NotifyBegin(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation,
	float TotalDuration
)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	if (IAttackTracing * ITrace = CheckTraceComponent(MeshComp)) 
	{ 
		ITrace->ToggleTrace(true);
	}
}

void UAttackTraceAnimNotifyState::NotifyEnd(
	USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation
)
{
	Super::NotifyEnd(MeshComp, Animation);

	if (IAttackTracing* ITrace = CheckTraceComponent(MeshComp)) 
	{
		ITrace->ToggleTrace(false);
	}
}

IAttackTracing* UAttackTraceAnimNotifyState::CheckTraceComponent(USkeletalMeshComponent* MeshComp)
{
	AActor* OwnerRef = MeshComp->GetOwner(); 

	bool hasTraceInterface = OwnerRef->GetClass() 
		->ImplementsInterface(UAttackTracing::StaticClass()); 

	if (!hasTraceInterface) { return nullptr; }

	return Cast<IAttackTracing>(OwnerRef); 
}
