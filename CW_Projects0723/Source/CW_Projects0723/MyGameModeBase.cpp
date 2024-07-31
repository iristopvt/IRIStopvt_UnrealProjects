// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "MyPawn.h"
#include "MyCharacter.h"

AMyGameModeBase::AMyGameModeBase()
{
	DefaultPawnClass = AMyPawn::StaticClass();

	static ConstructorHelpers::FClassFinder<AMyCharacter>
		myCharacter(TEXT("/Script/Engine.Blueprint'/Game/BulePrint/Player/MyCharacter_BP.MyCharacter_BP_C'"));
	
	if (myCharacter.Succeeded())
	{
		DefaultPawnClass = myCharacter.Class;
	}

	static ConstructorHelpers::FClassFinder<AMyCharacter> myMonster
	(TEXT("/Script/Engine.Blueprint'/Game/BulePrint/Player/MyMonster_BP.MyMonster_BP_C'"));
	if (myMonster.Succeeded())
	{
		_monsterClass = myMonster.Class;
	}
}

void AMyGameModeBase::BeginPlay()
{
	// ���� ���� ��ġ
	FVector location = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation()+ FVector(100,0.0f,0.0f);
	FRotator rotator = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorRotation();

	// �ݺ������� ������ ������ ����
	for (int i = 0; i < 3; i++)
	{
		location.X += 100.0 * i;

		AMyCharacter* monster = GetWorld()->SpawnActor<AMyCharacter>(_monsterClass, location,rotator);
		_monster.Add(monster);
	}
}
