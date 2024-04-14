// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyProjectileComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MELEECOMBATDEMO_API UEnemyProjectileComponent : public UActorComponent
{
	GENERATED_BODY()

	USceneComponent* SpawnPointComp;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	FName SpawnPointComponentName;

public:	
	UFUNCTION(BlueprintCallable)
	void SpawnProjectile(TSubclassOf<class AEnemyProjectile> ProjectileClass);

};