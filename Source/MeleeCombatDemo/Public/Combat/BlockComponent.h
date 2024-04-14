// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/Combat.h"
#include "Interfaces/MainPlayer.h"
#include "BlockComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(
	FOnBlockSignature,
	UBlockComponent, OnBlockDelegate,
	float, Cost
);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MELEECOMBATDEMO_API UBlockComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float StaminaCost{ 10.0f };

	ACharacter* OwnerRef;

	ICombat* IFighterRef;

	IMainPlayer* IPlayerRef;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	UPROPERTY(BlueprintAssignable)
	FOnBlockSignature OnBlockDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* ImpactAnimMontage;

	bool Check(AActor* Opponent);
};
