// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyPlayerAnimInstance.generated.h"

/**
 * 
 */


DECLARE_MULTICAST_DELEGATE(AttackDelegate);

UCLASS()
class CW_PROJECTS0723_API UMyPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UMyPlayerAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconde) override;

	virtual void PlayAttackMontage();


	virtual void JumpToSection(int32 sectionIndex);

	UFUNCTION()
	virtual void AnimNotify_Attackhit();

	AttackDelegate _attackDelegate;

	void PlayDeath();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = pawen, Meta = (AllowPrivateAccess = true))
	float _speed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = pawen, Meta = (AllowPrivateAccess = true))
	bool _isFalling;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = pawen, Meta = (AllowPrivateAccess = true))
	bool _isDead;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = pawen, Meta = (AllowPrivateAccess = true))
	float _vartical;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = pawen, Meta = (AllowPrivateAccess = true))
	float _horizontal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = pawen, Meta = (AllowPrivateAccess = true))
	class UAnimMontage* _myplAnimMontage;


};
