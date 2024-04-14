// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LockonComponent.generated.h"

class ULockonComponent;

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(
	FOnUpdatedTargetSignature, 
	ULockonComponent, OnUpdatedTargetDelegate, 
	AActor*, NewTargetActorRef
);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MELEECOMBATDEMO_API ULockonComponent : public UActorComponent
{
	GENERATED_BODY()

	AActor* ActorRef;

	TArray<AActor*> Targets;

	class USpringArmComponent* SpringArmComp;

public:	
	// Sets default values for this component's properties
	ULockonComponent();

	UPROPERTY(BlueprintReadOnly)
	AActor* CurrentTargetActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	double BreakDistance{ 1000.0 };

	UPROPERTY(BlueprintAssignable)
	FOnUpdatedTargetSignature OnUpdatedTargetDelegate;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void StartLockon(float Radius);

	UFUNCTION(BlueprintCallable)
	void EndLockon();

	UFUNCTION(BlueprintCallable)
	void ToggleLockon(float radius);

	UFUNCTION(BlueprintCallable)
	void SwitchTarget(enum ELockonDirection InputDirection);
};
