// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyAIController.generated.h"

/**
 * 
 */
UCLASS()
class CW_PROJECTS0723_API AMyAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	AMyAIController();
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

	void RandMove();

private:
	//FTimerHandle _timerHandle;

	// 따로 있는걸 AI컨트롤러에서 묶어줌 
	UPROPERTY()
	class  UBlackboardData* _bb; //블랙보드
	UPROPERTY()
	class UBehaviorTree* _bt; // 블랙트리 

};
