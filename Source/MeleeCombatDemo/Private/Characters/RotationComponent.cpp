// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RotationComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
URotationComponent::URotationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URotationComponent::BeginPlay()
{
	Super::BeginPlay();

	ActorRef = GetOwner();
}


// Called every frame
void URotationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bCanRotate) { return; }

	FVector OwnerLoc = ActorRef->GetActorLocation();
	FVector PlayerLoc = GetWorld()->GetFirstPlayerController()
		->GetPawn()
		->GetActorLocation();

	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(OwnerLoc, PlayerLoc);
	FRotator CurrentRotation = ActorRef->GetActorRotation();

	FRotator NewRotation = UKismetMathLibrary::RInterpTo_Constant(
		LookAtRotation, CurrentRotation, DeltaTime, Speed
	);

	FRotator NewYawOnlyRotation{ 
		CurrentRotation.Pitch, NewRotation.Yaw, CurrentRotation.Roll
	};

	ActorRef->SetActorRotation(NewRotation);
}

