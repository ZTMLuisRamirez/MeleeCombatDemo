// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/EnemyProjectileComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Combat/EnemyProjectile.h"

// Sets default values for this component's properties
UEnemyProjectileComponent::UEnemyProjectileComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyProjectileComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	SpawnPointComp = Cast<USceneComponent>(
		GetOwner()->GetDefaultSubobjectByName(SpawnPointComponentName)
	);
}


// Called every frame
void UEnemyProjectileComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

//void UEnemyProjectileComponent::StartAttack(UAnimMontage* AnimMontage)
//{
//	ACharacter* CharacterRef = GetOwner<ACharacter>();
//
//	float Duration = CharacterRef->PlayAnimMontage(AnimMontage);
//}

void UEnemyProjectileComponent::SpawnProjectile(TSubclassOf<AEnemyProjectile> ProjectileClass)
{
	FVector SpawnLocation = SpawnPointComp->GetComponentLocation();
	FVector PlayerLocation = GetWorld()->GetFirstPlayerController()
		->GetPawn()
		->GetTargetLocation();

	FRotator SpawnRotation = UKismetMathLibrary::FindLookAtRotation(
		SpawnLocation, PlayerLocation
	);

	FActorSpawnParameters Params;

	GetWorld()->SpawnActor(
		ProjectileClass, &SpawnLocation, &SpawnRotation, Params
	);
}

