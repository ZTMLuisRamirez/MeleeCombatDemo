// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LockonComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MELEECOMBATDEMO_API ULockonComponent : public UActorComponent
{
	GENERATED_BODY()

	AActor* ActorRef;

public:	
	// Sets default values for this component's properties
	ULockonComponent();

	TArray<AActor*> Targets;

	UPROPERTY(BlueprintReadOnly)
	AActor* CurrentTargetActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bShouldAutofocus{ true };

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
};
