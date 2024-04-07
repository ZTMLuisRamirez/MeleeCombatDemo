// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/StatsComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UStatsComponent::UStatsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStatsComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStatsComponent::RegenStamina()
{
	if (!bCanRegen) 
	{ 
		OnUpdateStatDelegate.Broadcast(StatType::Stamina, Stats[StatType::Stamina]);
		return; 
	}

	if (!Stats.Contains(StatType::Stamina) || !Stats.Contains(StatType::MaxStamina))
	{
		return;
	}

	Stats[StatType::Stamina] = UKismetMathLibrary::FInterpTo_Constant(
		Stats[StatType::Stamina],
		Stats[StatType::MaxStamina],
		GetWorld()->DeltaTimeSeconds,
		StaminaRegenRate
	);

	OnUpdateStatDelegate.Broadcast(StatType::Stamina, Stats[StatType::Stamina]);
}

void UStatsComponent::HandleAttackPerformed(float Amount)
{
	bCanRegen = false;

	Stats[StatType::Stamina] -= Amount;
	Stats[StatType::Stamina] = UKismetMathLibrary::Clamp(
		Stats[StatType::Stamina],
		0,
		Stats[StatType::MaxStamina]
	);
}

void UStatsComponent::HandleAttackComplete()
{
	bCanRegen = true;
}
