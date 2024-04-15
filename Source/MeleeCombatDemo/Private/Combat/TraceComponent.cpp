// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/TraceComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Characters/StatsComponent.h"
#include "Interfaces/Combat.h"
#include "Kismet/GameplayStatics.h"

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

	TArray<FHitResult> AllResults{ };
	bool hasFoundTargets = false;

	for (const FTraceSockets Socket : Sockets)
	{
		TArray<FHitResult> OutResults;

		// Get Socket Locations
		FVector StartSocketLocation{ SkeletalComp->GetSocketLocation(Socket.Start) };
		FVector EndSocketLocation{ SkeletalComp->GetSocketLocation(Socket.End) };
		FQuat ShapeRotation{ SkeletalComp->GetSocketQuaternion(Socket.Rotation) };

		// list of actors to search for!
		FCollisionObjectQueryParams CollisionObjectParams{ ECC_Pawn };

		FVector WeaponDistance{ StartSocketLocation - EndSocketLocation }; 
		FVector BoxHalfExtent{ 
			BoxCollisionLength, BoxCollisionLength, WeaponDistance.Size() / 2
		};
		FCollisionShape CollisionBox{ FCollisionShape::MakeBox(BoxHalfExtent) }; 

		// list of actors to ignore
		FCollisionQueryParams TraceParams{ 
			FName(TEXT("CollisionParams")), false, PawnRef
		}; 
		hasFoundTargets = GetWorld()->SweepMultiByObjectType(
			OutResults,
			StartSocketLocation,
			EndSocketLocation,
			ShapeRotation,
			CollisionObjectParams,
			CollisionBox,
			TraceParams
		);

		for (FHitResult Hit : OutResults)
		{
			
			AllResults.Add(Hit);
		}

		if (bDebugMode)
		{
			FVector CenterPoint{ UKismetMathLibrary::VLerp(
				StartSocketLocation, EndSocketLocation, 0.5f
			) };

			UKismetSystemLibrary::DrawDebugBox(
				GetWorld(),
				CenterPoint,
				CollisionBox.GetExtent(),
				hasFoundTargets ? FColor::Green : FColor::Red, 
				ShapeRotation.Rotator(),
				3.0f, // Duration
				2.0f // Thickness
			);
		}
	}
	
	if (AllResults.Num() <= 0) { return; }

	float CharacterDamage = 0.0f;
	ICombat* CombatRef = Cast<ICombat>(PawnRef);
	
	if (CombatRef)
	{
		CharacterDamage = CombatRef->GetDamage();
	}
	
	FDamageEvent TargetAttackedEvent{ };

	for (const FHitResult Hit : AllResults)
	{
		AActor* TargetActor = Hit.GetActor();

		ICombat* ITargetCombatRef = Cast<ICombat>(TargetActor);

		if (ITargetCombatRef && ITargetCombatRef->IsRolling()){ continue; }

		if (IgnoreTargets.Contains(TargetActor) || TargetActor->ActorHasTag(IgnoreActorTag)) { continue; }
		
		TargetActor->TakeDamage(
			CharacterDamage,
			TargetAttackedEvent,
			GetOwner()->GetInstigatorController(),
			PawnRef
		);

		IgnoreTargets.AddUnique(TargetActor);

		// Spawn Bloods
		UGameplayStatics::SpawnEmitterAtLocation(
			GetWorld(),
			HitParticlesTemplate,
			Hit.Location,
			UKismetMathLibrary::MakeRotFromZ(Hit.Normal)
		);
	}
}

void UTraceComponent::HandleResetAttack()
{
	IgnoreTargets.Empty();
}