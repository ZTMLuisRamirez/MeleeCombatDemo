// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/EnemyProjectileComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Combat/EnemyProjectile.h"

// Called when the game starts
void UEnemyProjectileComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	SpawnPointComp = Cast<USceneComponent>(
		GetOwner()->GetDefaultSubobjectByName(SpawnPointComponentName)
	);
}

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

