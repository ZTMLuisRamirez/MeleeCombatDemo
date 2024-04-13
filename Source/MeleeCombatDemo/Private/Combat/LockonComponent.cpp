// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/LockonComponent.h"
#include "Interfaces/Enemy.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Combat/ELockonDirection.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values for this component's properties
ULockonComponent::ULockonComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULockonComponent::BeginPlay()
{
	Super::BeginPlay();

	ActorRef = GetOwner();

	SpringArmComp = ActorRef->FindComponentByClass<USpringArmComponent>();
}


// Called every frame
void ULockonComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bShouldAutofocus) { return; }

	// Check if there's a target to focus
	if (!IsValid(CurrentTargetActor)) { return; }

	FVector CurrentLocation{ ActorRef->GetActorLocation() };
	FVector TargetLocation{ CurrentTargetActor->GetActorLocation() };

	// Check if target is too far away and break lockon if they are
	double TargetDistance{
		FVector::Distance(CurrentLocation, TargetLocation)
	};

	if (TargetDistance >= BreakDistance)
	{
		EndLockon();
		return;
	}

	// Otherwise, rotate toward current lockon target
	FVector LowerTargetLocation{
		TargetLocation.X, TargetLocation.Y, TargetLocation.Z - 125
	};

	FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(
		CurrentLocation, LowerTargetLocation
	);

	GetWorld()->GetFirstPlayerController()->SetControlRotation(NewRotation);

}

void ULockonComponent::StartLockon(float Radius)
{
	// Create a Collision Shape to Detect Targets
	TArray<FHitResult> OutResults;
	FVector CurrentLocation = ActorRef->GetActorLocation();
	FCollisionObjectQueryParams QueryParams{ ECC_Pawn };
	FCollisionShape CollisionSphere = FCollisionShape::MakeSphere(Radius);

	bool hasFoundTargets = GetWorld()->SweepMultiByObjectType(
		OutResults,
		CurrentLocation,
		CurrentLocation,
		FQuat::Identity,
		QueryParams,
		CollisionSphere
	);

	if (!hasFoundTargets) { return; }

	// Check if targets impelement interface
	// https://stackoverflow.com/a/15176127
	for (FHitResult const& result : OutResults)
	{
		AActor* ActorResultRef = result.GetActor();
		bool hasLockonInterface = ActorResultRef->GetClass()
			->ImplementsInterface(UEnemy::StaticClass());

		if (!hasLockonInterface) { continue; }

		IEnemy* ITargetable = Cast<IEnemy>(
			ActorResultRef
		);
		
		if (ITargetable && ITargetable->IsDead()) { continue; }

		// Add target to array of possible targets
		Targets.AddUnique(ActorResultRef);
	}

	// Start searching for nearest target
	double ClosestTargetDistance{ std::numeric_limits<double>::max() };

	for (AActor* target : Targets)
	{
		FVector TargetLocation = target->GetActorLocation();

		double TargetDistance = FVector::Distance(
			CurrentLocation, TargetLocation
		);

		if (TargetDistance <= ClosestTargetDistance)
		{
			ClosestTargetDistance = TargetDistance;
			CurrentTargetActor = target;
		}
	}

	if (!IsValid(CurrentTargetActor)) { return; }

	APlayerController* Controller{ GetWorld()->GetFirstPlayerController() };
	Controller->SetIgnoreLookInput(true);

	UCharacterMovementComponent* MovementComp{
		Controller->GetCharacter()->GetCharacterMovement()
	};

	MovementComp->bOrientRotationToMovement = false;
	MovementComp->bUseControllerDesiredRotation = true;

	// Adjust Camera
	SpringArmComp->TargetOffset = FVector{ 0.0, 0.0, 100.0 };

	IEnemy* ITargetable = Cast<IEnemy>(
		CurrentTargetActor
	);

	if (ITargetable) { ITargetable->OnSelect(); }

	OnUpdatedTargetDelegate.Broadcast(CurrentTargetActor);
}

void ULockonComponent::EndLockon()
{
	IEnemy* ITarget = Cast<IEnemy>(CurrentTargetActor);

	ITarget->OnDeselect();

	Targets.Empty();
	CurrentTargetActor = nullptr;

	APlayerController* Controller{ GetWorld()->GetFirstPlayerController() };
	Controller->ResetIgnoreLookInput();

	UCharacterMovementComponent* MovementComp{
		Controller->GetCharacter()->GetCharacterMovement()
	};

	MovementComp->bOrientRotationToMovement = true;
	MovementComp->bUseControllerDesiredRotation = false;

	// Adjust Camera
	SpringArmComp->TargetOffset = FVector::ZeroVector;

	OnUpdatedTargetDelegate.Broadcast(CurrentTargetActor);
}

void ULockonComponent::ToggleLockon(float radius)
{
	if (IsValid(CurrentTargetActor))
	{
		EndLockon();
	}
	else
	{
		StartLockon(radius);
	}
}

void ULockonComponent::SwitchTarget(ELockonDirection InputDirection)
{
	if (!IsValid(CurrentTargetActor)) { return; }

	AActor* OriginalTarget{ CurrentTargetActor };
	Targets.Remove(CurrentTargetActor);

	bool bHasFoundNewTarget{ false };

	double ClosestSwitchTargetDistance{ std::numeric_limits<double>::max() };
	FVector CurrentLocation{ CurrentTargetActor->GetActorLocation() };

	for (AActor* PotentialTarget : Targets)
	{
		FVector PotentialTargetLocation{ PotentialTarget->GetActorLocation() };
		FVector Difference{ CurrentLocation - PotentialTargetLocation };
		Difference.Normalize();

		float result{ static_cast<float>(
			FVector::DotProduct(
				CurrentTargetActor->GetActorRightVector(), Difference
			)
		) };

		if (
			(result < 0 && InputDirection == ELockonDirection::Left) ||
			(result > 0 && InputDirection == ELockonDirection::Right)
		)
		{
			continue;
		}

		double TargetDistance{ 
			FVector::Distance(CurrentLocation, PotentialTargetLocation)
		};

		if (TargetDistance <= ClosestSwitchTargetDistance)
		{
			IEnemy* IOriginalTarget = Cast<IEnemy>(CurrentTargetActor);
			
			if (IOriginalTarget) { IOriginalTarget->OnDeselect(); }
		
			ClosestSwitchTargetDistance = TargetDistance;
			CurrentTargetActor = PotentialTarget;
			bHasFoundNewTarget = true;

			IEnemy* INewTarget = Cast<IEnemy>(CurrentTargetActor);
			
			if (INewTarget) { INewTarget->OnSelect(); }
		}

		/*UE_LOG(
			LogClass,
			Warning,
			TEXT("Target: %s Result: %f"),
			*potentialTarget->GetActorNameOrLabel(),
			result
		);*/
	}

	if (bHasFoundNewTarget)
	{
		Targets.AddUnique(OriginalTarget);
		OnUpdatedTargetDelegate.Broadcast(CurrentTargetActor);
	}
}
