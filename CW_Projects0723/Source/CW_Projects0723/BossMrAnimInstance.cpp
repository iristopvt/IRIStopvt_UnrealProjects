// Fill out your copyright notice in the Description page of Project Settings.


#include "BossMrAnimInstance.h"
#include "MyBossMonster.h"
#include "MyCharacter.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Animation/AnimMontage.h"

UBossMrAnimInstance::UBossMrAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> bo
	(TEXT("/Script/Engine.AnimMontage'/Game/BulePrint/Animation/Monster/BossMontage_01.BossMontage_01'"));

	if (bo.Succeeded())
	{
		_BossAnimMontage = bo.Object;
	}

}

void UBossMrAnimInstance::NativeUpdateAnimation(float DeltaSeconde)
{
	AMyBossMonster* Boss = Cast<AMyBossMonster>(TryGetPawnOwner());
	if (Boss != nullptr)
	{
		_speed = Boss->GetVelocity().Size();
		_isFalling = Boss->GetMovementComponent()->IsFalling();
		_vartical = _vartical + (Boss->_varical - _vartical) * (DeltaSeconde);
		_horizontal = _horizontal + (Boss->_horizontal - _horizontal) * (DeltaSeconde);
		_isDead = (Boss->GetCurHp() <= 0);
	}
}

void UBossMrAnimInstance::BossAttackMontage()
{
	if (!Montage_IsPlaying(_BossAnimMontage))
	{
		Montage_Play(_BossAnimMontage);

		AMyBossMonster* BossCharacter = Cast<AMyBossMonster>(TryGetPawnOwner());

	}
}

void UBossMrAnimInstance::JumpToSection(int32 sectionIndex)
{
	FName sectionName = FName(*FString::Printf(TEXT("Attack%d"), sectionIndex));
	Montage_JumpToSection(sectionName);
}

void UBossMrAnimInstance::AnimNotify_Attackhit()
{
	_BossattackDelegate.Broadcast();

}

