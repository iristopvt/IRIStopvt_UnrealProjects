// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MyGameModeBase.generated.h"


/**
 * 
 */
UCLASS()
class CW_PROJECTS0723_API AMyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	AMyGameModeBase();
	
	virtual void BeginPlay() override;
	UPROPERTY()
	UClass* _monsterClass;
	
	//TArray�� �𸮾󿡼� vector 
	UPROPERTY()
	TArray<class AMyCharacter*> _monster;



};
