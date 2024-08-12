// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MyMonsterAnimInstance.generated.h"

/**
 * 
 */

DECLARE_MULTICAST_DELEGATE(AttackDelegate);
DECLARE_MULTICAST_DELEGATE(DeathDelegate);

UCLASS()
class CW_PROJECTS0723_API UMyMonsterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	

public:
	UMyMonsterAnimInstance();

	virtual void NativeUpdateAnimation(float DeltaSeconde) override;

	virtual void PlayAttackMontage();
	void AnAttackMontage();

	virtual void JumpToSection(int32 sectionIndex);

	UFUNCTION()
	virtual void AnimNotify_Attackhit();

	UFUNCTION()
	void AnimNotify_Death();

	AttackDelegate _attackDelegate;
	DeathDelegate _deathDelegate;

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
	class UAnimMontage* _myMoAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = pawen, Meta = (AllowPrivateAccess = true))
	class UAnimMontage* _anMonAnimMontage;
};
