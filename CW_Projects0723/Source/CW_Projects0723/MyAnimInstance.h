// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(AttackDelegate);
DECLARE_MULTICAST_DELEGATE(DeathDelegate);

/**
 * 
 */
UCLASS()
class CW_PROJECTS0723_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UMyAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconde) override;

	void PlayAttackMontage();
	void PlayDeath();
	void DelegateTest();
	void DelegateTest2(int32 hp, int32 mp);

	void JumpToSection(int32 sectionIndex);

	UFUNCTION()
	void AnimNotify_Attackhit();

	AttackDelegate _attackDelegate;
	DeathDelegate _deathDelegate;

	UFUNCTION()
	void AnimNotify_Death();

private:
	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category = pawen, Meta = (AllowPrivateAccess = true))
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
	class UAnimMontage* _myAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = pawen, Meta = (AllowPrivateAccess = true))
	class UAnimMontage* _myAnimDeathMontage;
};
