// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyCharacter.h"
#include "MyMonster.generated.h"

/**
 * 
 */


UCLASS()
class CW_PROJECTS0723_API AMyMonster : public AMyCharacter
{
	GENERATED_BODY()

public:
	AMyMonster();
	virtual void PostInitializeComponents() override;
	virtual void Attackhit();

	virtual void BeginPlay() override;

	void Attack_AI();
	virtual void Disable() override;
	class UMyMonsterAnimInstance* _MoanimInstance;

};