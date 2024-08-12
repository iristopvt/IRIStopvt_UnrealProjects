// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyCharacter.h"
#include "MyBossMonster.generated.h"

/**
 * 
 */
UCLASS()
class CW_PROJECTS0723_API AMyBossMonster : public AMyCharacter
{
	GENERATED_BODY()
public:
	AMyBossMonster();
	virtual void PostInitializeComponents() override;
	
	virtual void Attackhit();

	virtual void BeginPlay() override;

	void Attack_AI();

	class UBossMrAnimInstance* _BossanimInstance;

};
