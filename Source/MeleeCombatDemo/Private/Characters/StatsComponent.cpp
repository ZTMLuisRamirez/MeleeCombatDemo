// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/StatsComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

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
	if (!bCanRegen) { return; }

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

	OnUpdateStaminaDelegate.Broadcast(
		Stats[StatType::Stamina], Stats[StatType::MaxStamina]
	);
}

void UStatsComponent::DelayStaminaRegen()
{
	FLatentActionInfo FunctionInfo;
	FunctionInfo.CallbackTarget = this;
	FunctionInfo.ExecutionFunction = "EnableRegen";
	FunctionInfo.Linkage = 0; 
	FunctionInfo.UUID = 100; 

	UKismetSystemLibrary::RetriggerableDelay(
		GetWorld(),
		StaminaDelayTime,
		FunctionInfo
	);
}

void UStatsComponent::EnableRegen()
{
	bCanRegen = true;
}

void UStatsComponent::BroadcastHealthUpdate()
{
	OnHealthUpdateDelegate.Broadcast(
		Stats[StatType::Health], Stats[StatType::MaxHealth]
	);
}

void UStatsComponent::ReduceStamina(float Amount)
{
	bCanRegen = false;

	Stats[StatType::Stamina] -= Amount;
	Stats[StatType::Stamina] = UKismetMathLibrary::FClamp(
		Stats[StatType::Stamina],
		0,
		Stats[StatType::MaxStamina]
	);

	OnUpdateStaminaDelegate.Broadcast(
		Stats[StatType::Stamina], Stats[StatType::MaxStamina]
	);
}

void UStatsComponent::ReduceHealth(float Amount)
{
	Stats[StatType::Health] -= Amount;
	Stats[StatType::Health] = UKismetMathLibrary::FClamp(
		Stats[StatType::Health],
		0,
		Stats[StatType::MaxHealth]
	);

	OnHealthUpdateDelegate.Broadcast(
		Stats[StatType::Health], Stats[StatType::MaxHealth]
	);

	if (Stats[StatType::Health] <= 0)
	{
		OnZeroHealthDelegate.Broadcast();
	}
}
