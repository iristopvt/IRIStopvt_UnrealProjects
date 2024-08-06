// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyStatComponent.h"

#include "MyGameInstance.generated.h"

/**
 * 
 */
class AMyUIManager;

#define UIManager Cast<UMyGameInstance>(GetGameInstance())->GetUIManager()

UCLASS()
class CW_PROJECTS0723_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UMyGameInstance();

	virtual void Init() override;

	FMystatData* GetStatDataMyLevel(int level);

	AMyUIManager* GetUIManager() { return _uiManger; }

private:
	UPROPERTY()
	class UDataTable* _statTable;

	UPROPERTY()
	 AMyUIManager* _uiManger;

	 bool bIsInitlalized = false;
	
};
