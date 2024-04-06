// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/LockonComponent.h"
#include "Interfaces/TargetableInterface.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

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

	//if (TargetDistance >= BreakDistance)
	//{
	//	EndLockon();
	//	return;
	//}

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
			->ImplementsInterface(UTargetableInterface::StaticClass());

		if (!hasLockonInterface) { continue; }

		//ITargetableInterface* ITarget = Cast<ITargetableInterface>(
		//	ActorResultRef
		//);
		//
		//if (ITarget && ITarget->IsDead()) { continue; }

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

	//ILockOnTargetInterface* TargetInterface = Cast<ILockOnTargetInterface>(TargetActor);

	//TargetInterface->OnSelect();
}

void ULockonComponent::EndLockon()
{
	//ITargetableInterface* ITarget = Cast<ITargetableInterface>(ActorRef);

	/*ITarget->OnDeselect();*/

	Targets.Empty();
	CurrentTargetActor = nullptr;

	APlayerController* Controller{ GetWorld()->GetFirstPlayerController() };
	Controller->ResetIgnoreLookInput();

	UCharacterMovementComponent* MovementComp{
		Controller->GetCharacter()->GetCharacterMovement()
	};

	MovementComp->bOrientRotationToMovement = true;
	MovementComp->bUseControllerDesiredRotation = false;
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

