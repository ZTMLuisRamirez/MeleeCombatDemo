// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/AttackComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Interfaces/Stamina.h"

// Sets default values for this component's properties
UAttackComponent::UAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
}


// Called every frame
void UAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAttackComponent::ComboAttack()
{
	AActor* ActorRef = GetOwner();
	bool hasLockonInterface = ActorRef->GetClass()
		->ImplementsInterface(UStamina::StaticClass());

	if (hasLockonInterface) 
	{ 
		IStamina* IStaminaRef = Cast<IStamina>(ActorRef);

		if (IStaminaRef && !IStaminaRef->HasEnoughStamina(StaminaCost)) { return; }
	}

	if (!bCanAttack) { return; }

	bCanAttack = false;

	int32 MaxCombo = AttackAnimations.Num();

	OwnerCharacter->PlayAnimMontage(AttackAnimations[ComboCounter]);

	ComboCounter++;

	ComboCounter = UKismetMathLibrary::Wrap(
		ComboCounter, -1, (MaxCombo - 1)
	);

	OnAttackPerformedDelegate.Broadcast(StaminaCost);
}

void UAttackComponent::HandleResetAttackCombo()
{
	ComboCounter = 0;
	OnAttackCompleteDelegate.Broadcast();
}

void UAttackComponent::HandleResetAttack()
{
	bCanAttack = true;
}

void UAttackComponent::RandomAttack()
{
	/*if (!CanAttack || CurrentStamina < AttackStaminaCost) { return; }

	CanAttack = false;*/

	int RandomIndex = FMath::RandRange(0, AttackAnimations.Num() - 1);

	AnimDuration = OwnerCharacter->PlayAnimMontage(AttackAnimations[RandomIndex]);
}

void UAttackComponent::BroadcastBlockDelegate()
{
	OnBlockDelegate.Broadcast(BlockStaminaCost);
}
