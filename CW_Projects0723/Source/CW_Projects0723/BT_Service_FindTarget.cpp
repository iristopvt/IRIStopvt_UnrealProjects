// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_Service_FindTarget.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "MyAIController.h"
#include "MyCharacter.h"
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"
#include "MyPlayer.h"

UBT_Service_FindTarget::UBT_Service_FindTarget()
{
	NodeName = TEXT("FindTarget");
	
	Interval = 0.5;
}

void UBT_Service_FindTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto currentPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (currentPawn == nullptr)
		return;

	auto world = GetWorld();
	FVector center = currentPawn->GetActorLocation();

	float searchRadius = 500.0f;

	if (world == nullptr)
		return;

	TArray<FOverlapResult> overLapResult;
	FCollisionQueryParams qparams(NAME_None, false, currentPawn);
	bool bResult = world->OverlapMultiByChannel
	(
		overLapResult,
		center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(searchRadius),
		qparams
	);



	///// 여기서 TODO 똑같이 터져서 gpt에 물어봄 
	//auto blackboardComponent = OwnerComp.GetBlackboardComponent();// TODO 
	//if (blackboardComponent == nullptr) // TODO 
	//	return; // TODO 


	//bool bFoundTarget = false; // TODO 

	if (bResult)
	{
		for (auto& result : overLapResult)
		{
			auto myCharacter = Cast<AMyPlayer>(result.GetActor());

			if (myCharacter != nullptr )
			{
				auto myCharacterContriller = myCharacter->GetController();
				if (myCharacterContriller != nullptr && myCharacterContriller->IsPlayerController())
				{

				OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), myCharacter);
				
				// DEBUG : DrawCapsule
				//DrawDebugSphere(world, center, searchRadius, 32, FColor::Red, false, 0.3f);
				
				}
				return;
			}

			//auto myCharacter = Cast<AMyPlayer>(result.GetActor());// TODO 
			//if (myCharacter != nullptr && myCharacter->GetController() && myCharacter->GetController()->IsPlayerController())
			//{
			//	blackboardComponent->SetValueAsObject(FName(TEXT("Target")), myCharacter);
			//	DrawDebugSphere(world, center, searchRadius, 32, FColor::Red, false, 0.3f);
			//	bFoundTarget = true;
			//	break; // Exit loop after finding the target// TODO 
			//}
		}
		// DEBUG : DrawCapsule

	// 	DrawDebugSphere(world, center, searchRadius, 32, FColor::Green, false, 0.3f);

		//if (!bFoundTarget)//TODO
		//{
		//	blackboardComponent->SetValueAsObject(FName(TEXT("Target")), nullptr);
		//	DrawDebugSphere(world, center, searchRadius, 32, FColor::Green, false, 0.3f);
		//} // TODO


	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(FName(TEXT("Target")), nullptr);
		
		// DEBUG : DrawCapsule
		//DrawDebugSphere(world, center, searchRadius, 32, FColor::Green, false, 0.3f);
	}
}
