// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBossMonster.h"
#include "MyAIController.h"
#include "MyAnimInstance.h"
#include "Engine/DamageEvents.h"
#include "BossMrAnimInstance.h"

AMyBossMonster::AMyBossMonster()
{
}

void AMyBossMonster::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	_statCom->SetLevellAndInit(5);

	_BossanimInstance = Cast<UBossMrAnimInstance>(GetMesh()->GetAnimInstance());
	if (_BossanimInstance->IsValidLowLevelFast())
	{
		_BossanimInstance->OnMontageEnded.AddDynamic(this, &AMyBossMonster::OnAttackEnded);
		_BossanimInstance->_BossattackDelegate.AddUObject(this, &AMyBossMonster::Attackhit);
		//_BossanimInstance->_deathDelegate.AddUObject(this, &AMyBossMonster::Disable);
	}
}

void AMyBossMonster::Attackhit()
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

void AMyBossMonster::BeginPlay()
{
	Super::BeginPlay();
	_aiController = Cast<AMyAIController>(GetController());
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		_BossanimInstance = Cast<UBossMrAnimInstance>(AnimInstance);
	}
}

void AMyBossMonster::Attack_AI()
{
	if (_statCom->IsDead())
		return;

	if (_isAttacking == false && _BossanimInstance != nullptr)
	{

		//_MoanimInstance->PlayAttackMontage(); // 거미 몽타주 공격
		_BossanimInstance->BossAttackMontage();
		_isAttacking = true;

		_curAttackIndex %= 3;
		_curAttackIndex++;


		_BossanimInstance->JumpToSection(_curAttackIndex);
	}
}
