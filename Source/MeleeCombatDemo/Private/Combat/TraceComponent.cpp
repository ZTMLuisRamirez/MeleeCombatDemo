// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/TraceComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Characters/StatsComponent.h"
#include "Interfaces/Combat.h"

// Sets default values for this component's properties
UTraceComponent::UTraceComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTraceComponent::BeginPlay()
{
	Super::BeginPlay();

	PawnRef = GetOwner<APawn>();
	SkeletalComp = PawnRef->FindComponentByClass<USkeletalMeshComponent>();
}


// Called every frame
void UTraceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bIsAttacking) { return; }

	TArray<FHitResult> OutResults;
	FVector StartSocketLocation{ SkeletalComp->GetSocketLocation(StartSocket) };
	FVector EndSocketLocation{ SkeletalComp->GetSocketLocation(EndSocket) };
	FQuat ShapeRotation{ SkeletalComp->GetSocketQuaternion(RotationSocket) };
	// list of actors to search for!
	FCollisionObjectQueryParams CollisionObjectParams{ ECC_Pawn };

	FVector SwordDistance{ StartSocketLocation - EndSocketLocation }; 
	float CapsuleHalfSize{ static_cast<float>(SwordDistance.Size() / 2) };
	FCollisionShape CollisionCapsule{ FCollisionShape::MakeCapsule( 
		Radius, CapsuleHalfSize
	) };

	// list of actors to ignore
	FCollisionQueryParams TraceParams{ 
		FName(TEXT("CollisionParams")), false, PawnRef
	};
	bool hasFoundTargets{ GetWorld()->SweepMultiByObjectType(
		OutResults,
		StartSocketLocation,
		EndSocketLocation,
		ShapeRotation,
		CollisionObjectParams,
		CollisionCapsule,
		TraceParams
	) };

	if (bDebugMode)
	{
		FVector CenterPoint{ UKismetMathLibrary::VLerp(
			StartSocketLocation, EndSocketLocation, 0.5f
		) };

		UKismetSystemLibrary::DrawDebugCapsule(
			GetWorld(),
			CenterPoint,
			CollisionCapsule.GetCapsuleHalfHeight(),
			Radius,
			ShapeRotation.Rotator(),
			hasFoundTargets ? FColor::Green : FColor::Red,
			//true,
			3.0f,
			//0,
			2.0f
		);
	}

	if (!hasFoundTargets) { return; }

	// To Fix

	float CharacterDamage = 0.0f;
	ICombat* CombatRef = Cast<ICombat>(PawnRef);
	
	if (CombatRef)
	{
		CharacterDamage = CombatRef->GetDamage();
	}
	
	FDamageEvent TargetAttackedEvent{ };

	for (FHitResult Hit : OutResults)
	{
		AActor* TargetActor = Hit.GetActor();
		
		if (IgnoreTargets.Contains(TargetActor)) { continue; }
		
		TargetActor->TakeDamage(
			CharacterDamage,
			TargetAttackedEvent,
			GetOwner()->GetInstigatorController(),
			PawnRef
		);

		IgnoreTargets.AddUnique(TargetActor);
	}
}

void UTraceComponent::HandleResetAttack()
{
	IgnoreTargets.Empty();
}