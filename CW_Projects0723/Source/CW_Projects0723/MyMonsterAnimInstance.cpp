// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMonster.h"
#include "MyMonsterAnimInstance.h"

#include "MyCharacter.h"

#include "GameFramework/PawnMovementComponent.h"
#include "Animation/AnimMontage.h"
UMyMonsterAnimInstance::UMyMonsterAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> mo
	(TEXT("/Script/Engine.AnimMontage'/Game/BulePrint/Animation/Monster/MySpiderAnimMontage.MySpiderAnimMontage'"));


	if (mo.Succeeded())
	{
		_myMoAnimMontage = mo.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> an
	(TEXT("/Script/Engine.AnimMontage'/Game/BulePrint/Animation/Monster/BosssoldierAnimMontage.BosssoldierAnimMontage'"));

	if (an.Succeeded())
	{
		_anMonAnimMontage = an.Object;
	}
}

void UMyMonsterAnimInstance::NativeUpdateAnimation(float DeltaSeconde)
{
	AMyMonster* myMonster = Cast<AMyMonster>(TryGetPawnOwner());
	if (myMonster != nullptr)
	{
		_speed = myMonster->GetVelocity().Size();
		_isFalling = myMonster->GetMovementComponent()->IsFalling();
		_vartical = _vartical + (myMonster->_varical - _vartical) * (DeltaSeconde);
		_horizontal = _horizontal + (myMonster->_horizontal - _horizontal) * (DeltaSeconde);
		_isDead = (myMonster->GetCurHp() <= 0);
	}
}

void UMyMonsterAnimInstance::PlayAttackMontage()
{
	if (!Montage_IsPlaying(_myMoAnimMontage))
	{
		Montage_Play(_myMoAnimMontage);

		AMyMonster* myMoCharacter = Cast<AMyMonster>(TryGetPawnOwner());

	}
}

void UMyMonsterAnimInstance::AnAttackMontage()
{
	if (!Montage_IsPlaying(_anMonAnimMontage))
	{
		Montage_Play(_anMonAnimMontage);

		AMyMonster* anMonster = Cast<AMyMonster>(TryGetPawnOwner());
	}
}

void UMyMonsterAnimInstance::JumpToSection(int32 sectionIndex)
{
	FName sectionName = FName(*FString::Printf(TEXT("Attack%d"), sectionIndex));
	Montage_JumpToSection(sectionName);
}

void UMyMonsterAnimInstance::AnimNotify_Attackhit()
{
	_attackDelegate.Broadcast();

}

void UMyMonsterAnimInstance::AnimNotify_Death()
{
	_deathDelegate.Broadcast();
}
