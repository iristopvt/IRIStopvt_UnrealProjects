// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_Task_Attack.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "MyAIController.h"
#include "MyCharacter.h"
#include "MyMonster.h"
#include "MyPlayer.h"

#include "MyBossMonster.h"
UBT_Task_Attack::UBT_Task_Attack()
{
	NodeName = TEXT("Attack");
}

EBTNodeResult::Type UBT_Task_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	
	auto character = Cast<AMyBossMonster>(OwnerComp.GetAIOwner()->GetPawn());
	//auto character = Cast<AMyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	if (character == nullptr)
		return EBTNodeResult::Failed;
	
	character->Attack_AI();
	_isAttacking = true;

	character->_attackEndendDelegate.AddLambda(
		[this]() -> void 
		{
			this->_isAttacking = false;
		});

	
	return result;
}

void UBT_Task_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (_isAttacking == false)
		FinishLatentTask(OwnerComp,EBTNodeResult::Succeeded);



	// 몬스터가 공격을 하면서 플레이어를 추적할려면 Tick 에다 이 코드를 넣어서 자연스럽게 ? 
	//auto target = Cast<AMyPlayer>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(FName(TEXT("Target"))));

	//if (character != nullptr && target != nullptr)
	//{
	//	// 몬스터가 캐릭터가 바라보는 방향을 계속 확인해 공격하는 코드
	//	FVector v = target->GetActorLocation() - character->GetActorLocation();
	//	v.Normalize();
	//	FQuat temp = FQuat::FindBetweenNormals(character->GetActorForwardVector(), v);

	//	character->SetActorRotation(FQuat::Slerp(character->GetActorQuat(), temp, 0.9f));

	//}
}


