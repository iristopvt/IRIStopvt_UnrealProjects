// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMonster.h"
#include "MyAIController.h"
#include "MyAnimInstance.h"
#include "MyMonsterAnimInstance.h"
#include "Engine/DamageEvents.h"

AMyMonster::AMyMonster()
{
}

void AMyMonster::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	_MoanimInstance = Cast<UMyMonsterAnimInstance>(GetMesh()->GetAnimInstance());
	if (_MoanimInstance->IsValidLowLevelFast())
	{
		_MoanimInstance->OnMontageEnded.AddDynamic(this, &AMyMonster::OnAttackEnded);
		_MoanimInstance->_attackDelegate.AddUObject(this, &AMyMonster::Attackhit);
		_MoanimInstance->_deathDelegate.AddUObject(this, &AMyMonster::Disable);
	}
}

void AMyMonster::Attackhit()
{
	FHitResult hitResult;
	FCollisionQueryParams params(NAME_None, false, this);

	float attackRange = 500.0f;
	float attackRadius = 150.0f;

	bool bResult = GetWorld()->SweepSingleByChannel
	(
		hitResult,
		GetActorLocation(),
		GetActorLocation() + GetActorForwardVector() * attackRange,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(attackRadius),
		params
	);

	FVector vec = GetActorForwardVector() * attackRange;
	FVector center = GetActorLocation() + vec * 0.5f;

	FColor drawColor = FColor::Green;


	if (bResult && hitResult.GetActor()->IsValidLowLevel())
	{
		drawColor = FColor::Red;

		FDamageEvent DamageEvent;
		hitResult.GetActor()->TakeDamage(_statCom->GetAttackDamage(), DamageEvent, GetController(), this);

	}

	DrawDebugSphere(GetWorld(), center, attackRadius, 12, drawColor, false, 2.0f);
}

void AMyMonster::BeginPlay()
{
	Super::BeginPlay();

	_aiController = Cast<AMyAIController>(GetController());
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		_MoanimInstance = Cast<UMyMonsterAnimInstance>(AnimInstance);
	}
}


void AMyMonster::Attack_AI()
{
	if (_statCom->IsDead())
		return;

	if (_isAttacking == false && _MoanimInstance != nullptr)
	{

		//_MoanimInstance->PlayAttackMontage(); // 거미 몽타주 공격
		_MoanimInstance->AnAttackMontage();
		_isAttacking = true;

		_curAttackIndex %= 3;
		_curAttackIndex++;


		_MoanimInstance->JumpToSection(_curAttackIndex);
	}
}

void AMyMonster::Disable()
{
	Super::Disable();
}


