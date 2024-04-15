// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BehaviorTree/BTS_PlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

void UBTS_PlayerLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FVector CurrentLocation = OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation();
	FVector PlayerLocation = GetWorld()->GetFirstPlayerController()
		->GetPawn()
		->GetActorLocation();

	double Distance = FVector::Dist(CurrentLocation, PlayerLocation);

	bool IsWithinRange = Distance < Range;

	//UE_LOG(
	//	LogClass, 
	//	Warning, 
	//	TEXT("Enemy: %s Player: %s Distance: %f, IsWithinRange: %s"), 
	//	*OwnerComp.GetAIOwner()->GetPawn()->GetName(),
	//	*GetWorld()->GetFirstPlayerController()->GetPawn()->GetName(),
	//	Distance,
	//	IsWithinRange ? TEXT("Yes") : TEXT("No")
	//);

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(
		BlackboardKey.SelectedKeyName,
		IsWithinRange
	);
}
