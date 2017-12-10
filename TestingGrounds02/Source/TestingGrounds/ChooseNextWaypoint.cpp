// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "AIController.h"
#include "PatrolRoute.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// TODO protect against no Patrol Route component


	// TODO protect against empty patrol points

	// get the patrol points
	auto ControlledPawn = OwnerComp.GetAIOwner()->GetPawn();
	auto PatrolRout = ControlledPawn->FindComponentByClass<UPatrolRoute>();

	if (!ensure(PatrolRout)) { return EBTNodeResult::Failed; }

	auto PatrolPoints = PatrolRout->GetPatrolPoints();

	if(PatrolPoints.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("A gaurd is missing patrol points."));
		return EBTNodeResult::Failed;
	}

	//set next waypoint 
	auto BlackboardComp = OwnerComp.GetBlackboardComponent();
	auto Index = BlackboardComp->GetValueAsInt(IndexKey.SelectedKeyName);
	BlackboardComp->SetValueAsObject(WaypointKey.SelectedKeyName, PatrolPoints[Index]);

	// cycle the index
	auto NextIndex = (Index + 1) % PatrolPoints.Num();
	BlackboardComp->SetValueAsInt(IndexKey.SelectedKeyName, NextIndex);

	return EBTNodeResult::Succeeded;
}
