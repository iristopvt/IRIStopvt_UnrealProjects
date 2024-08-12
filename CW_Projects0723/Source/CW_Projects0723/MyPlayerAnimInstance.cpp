// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerAnimInstance.h"
#include "MyPlayer.h"
#include "MyCharacter.h"

#include "GameFramework/PawnMovementComponent.h"
#include "Animation/AnimMontage.h"

UMyPlayerAnimInstance::UMyPlayerAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> pl
	(TEXT("/Script/Engine.AnimMontage'/Game/BulePrint/Animation/PlayerAnim/MyPlayerMontage.MyPlayerMontage'"));


	if (pl.Succeeded())
	{
		_myplAnimMontage = pl.Object;
	}
}

void UMyPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconde)
{
	AMyPlayer* myCharacter = Cast<AMyPlayer>(TryGetPawnOwner());
	if (myCharacter != nullptr)
	{
		_speed = myCharacter->GetVelocity().Size();
		_isFalling = myCharacter->GetMovementComponent()->IsFalling();
		_vartical = _vartical + (myCharacter->_varical - _vartical) * (DeltaSeconde);
		_horizontal = _horizontal + (myCharacter->_horizontal - _horizontal) * (DeltaSeconde);
		_isDead = (myCharacter->GetCurHp() <= 0);
	}
}

void UMyPlayerAnimInstance::PlayAttackMontage()
{
	if (!Montage_IsPlaying(_myplAnimMontage))
	{
		Montage_Play(_myplAnimMontage);
	}
}




void UMyPlayerAnimInstance::JumpToSection(int32 sectionIndex)
{
	FName sectionName = FName(*FString::Printf(TEXT("Attack%d"), sectionIndex));
	Montage_JumpToSection(sectionName);
}

void UMyPlayerAnimInstance::AnimNotify_Attackhit()
{
	_attackDelegate.Broadcast();

}

void UMyPlayerAnimInstance::PlayDeath()
{
}

