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

public:	
	// Sets default values for this component's properties
	UEnemyProjectileComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	FName SpawnPointComponentName;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//UFUNCTION(BlueprintCallable)
	//void StartAttack(UAnimMontage* AnimMontage);

	UFUNCTION(BlueprintCallable)
	void SpawnProjectile(TSubclassOf<class AEnemyProjectile> ProjectileClass);

};