// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIController.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

AMyAIController::AMyAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BB
	(TEXT("/Script/AIModule.BlackboardData'/Game/BulePrint/AI/MonsterBB_BP.MonsterBB_BP'"));
	if (BB.Succeeded())
	{
		_bb = BB.Object;
	}
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BT
	(TEXT("/Script/AIModule.BehaviorTree'/Game/BulePrint/AI/MonsterBT_BP.MonsterBT_BP'"));
	if (BT.Succeeded())
	{
		_bt = BT.Object;
	}
}

// 컨트롤러가 폰에 빙의하는 것을 possess 
void AMyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	// 타이머를 설정하여 일정 시간(3초)마다 함수호출 
	//GetWorld()->GetTimerManager().SetTimer(_timerHandle, this, &AMyAIController::RandMove, 3.0f, true);
	
	UBlackboardComponent* blackBoarComponent = Blackboard;
	if (UseBlackboard(_bb, blackBoarComponent))
	{
		if (RunBehaviorTree(_bt))
		{
			
			blackBoarComponent->SetValueAsVector(FName(TEXT("FixedPos")), InPawn->GetActorLocation());
			UE_LOG(LogTemp, Log, TEXT("Behavior Tree Succeeded"));
		}
	}
}

void AMyAIController::OnUnPossess()
{
	Super::OnUnPossess();
	// 타이머를 해제하여 더 이상 RandMove() 함수가 호출되지 않도록 함
	//GetWorld()->GetTimerManager().ClearTimer(_timerHandle);

	
}

void AMyAIController::RandMove()
{
	// AI가 움직이게 함 
	auto currentpawn = GetPawn();// 현재 컨트롤러가 제어하는 폰을 가져옴

	if (!currentpawn->IsValidLowLevel())// 폰이 유효하지 않으면 함수 종료
		return;

	auto naviSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld()); // 네비게이션 시스템 호출
	if (naviSystem == nullptr) // 내비게이션 시스템이 유효하지 않으면 함수 종료
		return;

	FNavLocation randLocation;// 무작위 위치를 저장할 변수
	if (naviSystem->GetRandomPointInNavigableRadius(FVector::ZeroVector, 500.0f, randLocation))
	{
		// AI를 무작위 위치로 이동시킴
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(this, randLocation);

	}
}
