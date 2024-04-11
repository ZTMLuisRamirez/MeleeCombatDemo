// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BehaviorTree/BTS_PlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"

void UBTS_PlayerLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FVector CurrentLocation = OwnerComp.GetOwner()->GetActorLocation();
	FVector PlayerLocation = GetWorld()->GetFirstPlayerController()
		->GetPawn()
		->GetActorLocation();

	double Distance = FVector::Dist(CurrentLocation, PlayerLocation);
	bool IsWithinRange = Distance < Range;

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(
		BlackboardKey.SelectedKeyName,
		IsWithinRange
	);
}
