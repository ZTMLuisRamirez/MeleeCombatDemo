// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyProjectile.generated.h"

UCLASS()
class MELEECOMBATDEMO_API AEnemyProjectile : public AActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float damage{ 10.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystem* HitTemplate;

public:	
	UFUNCTION(BlueprintCallable)
	void HandleBeginOverlap(AActor* OtherActor);

	UFUNCTION()
	void DestroyProjectile();

	UFUNCTION(BlueprintCallable)
	void BeginExplosion();
};
