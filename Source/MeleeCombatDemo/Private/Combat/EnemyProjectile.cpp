// Fill out your copyright notice in the Description page of Project Settings.


#include "Combat/EnemyProjectile.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"

void AEnemyProjectile::HandleBeginOverlap(AActor* OtherActor)
{
	APawn* PawnRef = Cast<APawn>(OtherActor);

	// Check if pawn is player controlled
	if (PawnRef == nullptr) { return; }

	if (!PawnRef->IsPlayerControlled()) { return; }

	// Apply Damage to Target
	FDamageEvent ProjectileAttackEvent{ };

	PawnRef->TakeDamage(
		damage,
		ProjectileAttackEvent,
		PawnRef->GetController(),
		this
	);

	FindComponentByClass<USphereComponent>()
		->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	BeginExplosion();
}

void AEnemyProjectile::DestroyProjectile()
{
	Destroy();
}

void AEnemyProjectile::BeginExplosion()
{
	// Stop projectile movement
	UProjectileMovementComponent* MovementComp = GetComponentByClass<UProjectileMovementComponent>();
	MovementComp->StopMovementImmediately();

	// Play Hit Template
	UParticleSystemComponent* ParticleComp = GetComponentByClass<UParticleSystemComponent>();
	ParticleComp->SetTemplate(HitTemplate);

	FTimerHandle DeathTimerHandler;
	GetWorldTimerManager().SetTimer(
		DeathTimerHandler,
		this,
		&AEnemyProjectile::DestroyProjectile,
		0.5f,
		false
	);
}

