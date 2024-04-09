// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PatrolComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UPatrolComponent::UPatrolComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPatrolComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPatrolComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

ATargetPoint* UPatrolComponent::GetCurrentPoint()
{
	return Points[CurrentPoint];
}

void UPatrolComponent::IncrementPoint()
{
	CurrentPoint++;

	CurrentPoint = static_cast<int>(UKismetMathLibrary::FWrap(
		CurrentPoint,
		-1,
		(Points.Num() - 1)
	));
}

