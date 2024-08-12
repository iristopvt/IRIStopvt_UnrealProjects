// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"
#include "MyCharacter.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Animation/AnimMontage.h"

UMyAnimInstance::UMyAnimInstance()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> am
	(TEXT("/Script/Engine.AnimMontage'/Game/BulePrint/Animation/MyAnimMontage.MyAnimMontage'"));

	//static ConstructorHelpers::FObjectFinder<UAnimMontage> die
	//(TEXT("/Script/Engine.AnimMontage'/Game/BulePrint/Animation/PlayerAnim/MyPlayerMontage.MyPlayerMontage'"));

	if (am.Succeeded())
	{
		_myAnimMontage = am.Object;
	}

	//if (die.Succeeded())
	//{
	//	_myAnimDeathMontage = die.Object;
	//}
}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconde)
{
	// ��¥ �߿�              ���̳��� ĳ��Ʈ(dynamic cast)
	AMyCharacter* myCharacter = Cast<AMyCharacter>(TryGetPawnOwner());
	if (myCharacter != nullptr)
	{
		_speed = myCharacter->GetVelocity().Size();
		_isFalling = myCharacter->GetMovementComponent()->IsFalling();
		_vartical = _vartical + (myCharacter->_varical - _vartical) * (DeltaSeconde);
		_horizontal = _horizontal + (myCharacter->_horizontal - _horizontal)*(DeltaSeconde);
		_isDead = (myCharacter->GetCurHp() <= 0);
	}

}

void UMyAnimInstance::PlayAttackMontage()
{
	if (!Montage_IsPlaying(_myAnimMontage))
	{
		Montage_Play(_myAnimMontage);

		AMyCharacter* myCharacter = Cast<AMyCharacter>(TryGetPawnOwner());
		
		// ������û�� �Ѵ�.
		//myCharacter->_myDelegate1.BindUObject(this, &UMyAnimInstance::DelegateTest);
		//myCharacter->_myDelegate3.BindUObject(this, &UMyAnimInstance::DelegateTest2);
		

	}

}

void UMyAnimInstance::PlayDeath()
{
	if (!Montage_IsPlaying(_myAnimDeathMontage))
	{
		Montage_Play(_myAnimDeathMontage);

			AMyCharacter* myCharacter = Cast<AMyCharacter>(TryGetPawnOwner());
	}
	

}

void UMyAnimInstance::DelegateTest()
{
	UE_LOG(LogTemp, Warning, TEXT("Delegate Test"));
}

void UMyAnimInstance::DelegateTest2(int32 hp, int32 mp)
{
	UE_LOG(LogTemp, Warning, TEXT("HP : %d, MP : %d "),hp,mp);
}

void UMyAnimInstance::JumpToSection(int32 sectionIndex)
{
	FName sectionName = FName(*FString::Printf(TEXT("Attack%d"), sectionIndex));
	Montage_JumpToSection(sectionName);
}

void UMyAnimInstance::AnimNotify_Attackhit()
{
	// ���� ����
	// �� �Լ����� ĳ���͵��� �浹ü ���� Ȥ�� ���尡 ���
	//UE_LOG(LogTemp, Warning, TEXT("Attack!!"));
	_attackDelegate.Broadcast();
}

void UMyAnimInstance::AnimNotify_Death()
{
	_deathDelegate.Broadcast();
}

