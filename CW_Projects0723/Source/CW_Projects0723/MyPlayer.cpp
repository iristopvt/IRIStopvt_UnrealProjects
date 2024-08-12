// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayer.h"
#include "EnhancedInputComponent.h"


// UI
#include "Components/CapsuleComponent.h"
// carmera
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Input
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "MyAnimInstance.h"
#include "MyPlayerAnimInstance.h"
#include "Engine/DamageEvents.h"

// _particle
#include "MyEffecManger.h"
#include "MyGameInstance.h"
#include "Sound/SoundCue.h"

AMyPlayer::AMyPlayer()
{


	_springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	_camera = CreateDefaultSubobject< UCameraComponent>(TEXT("Camera"));

	_springArm->SetupAttachment(GetCapsuleComponent());
	_camera->SetupAttachment(_springArm);

	_springArm->TargetArmLength = 500.0f;
	_springArm->SetRelativeRotation(FRotator(-35.0f, 0.0f, 0.0f));



}

void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();
	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		_PianimInstance = Cast<UMyPlayerAnimInstance>(AnimInstance);
	}
	
}

void AMyPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	_PianimInstance = Cast<UMyPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	if (_PianimInstance->IsValidLowLevelFast())
	{
		_PianimInstance->OnMontageEnded.AddDynamic(this, &AMyPlayer::OnAttackEnded);
		_PianimInstance->_attackDelegate.AddUObject(this, &AMyPlayer::Attackhit);
	///	_PianimInstance->_deathDelegate.AddUObject(this, &AMyPlayer::Disable);
	}
}

void AMyPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AMyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Moving
		EnhancedInputComponent->BindAction(_moveAction, ETriggerEvent::Triggered, this, &AMyPlayer::Move);

		// Looking
		EnhancedInputComponent->BindAction(_lookAction, ETriggerEvent::Triggered, this, &AMyPlayer::Look);

		// jumping
		EnhancedInputComponent->BindAction(_jumpAction, ETriggerEvent::Triggered, this, &AMyPlayer::JumpA);

		// Attack
		EnhancedInputComponent->BindAction(_attackAction, ETriggerEvent::Started, this, &AMyPlayer::AttackA);
	
		// chuck
		EnhancedInputComponent->BindAction(_itemDropAction, ETriggerEvent::Triggered, this, &AMyPlayer::DropItem);

	}
}

float AMyPlayer::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	float damaged = -_statCom->AddCurHp(-Damage);



	return damaged;
}

void AMyPlayer::Attackhit()
{
	/*FHitResult hitResult;
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

	DrawDebugSphere(GetWorld(), center, attackRadius, 12, drawColor, false, 2.0f);*/

	// 원거리 공격 
	FHitResult hitResult;
	FCollisionQueryParams params(NAME_None, false, this);

	float attackRange = 1000.0f;
	float attackRadius = 20.0f; 

	FVector forward = GetActorForwardVector();
	FQuat quat = FQuat::FindBetweenVectors(FVector(0, 0, 1), forward); // FRotationMatrix::MakeFromZ(forward).ToQuat();

	FVector center = GetActorLocation() + forward * attackRange * 0.5f;
	FVector start = GetActorLocation();
	FVector end = GetActorLocation() + forward * (attackRange*0.5f);

	bool bResult = GetWorld()->SweepSingleByChannel
	(
		hitResult,
		start,
		end,
		quat,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeCapsule(attackRadius, attackRange* 0.5f),
		params
	);



	FColor drawColor = FColor::Green;

	// Check if a hit was detected and apply damage if valid
	if (bResult && hitResult.GetActor()->IsValidLowLevel())
	{
		drawColor = FColor::Red;
		FDamageEvent damageEvent;
		hitResult.GetActor()->TakeDamage(_statCom->GetAttackDamage(), damageEvent, GetController(), this);
		
		FVector hitPoint = hitResult.ImpactPoint; // 파티파티클 시스템 

		_hitPoint = hitResult.ImpactPoint;

		//_attackHitEvent.Broadcast();
		EffecManger->Play("MeleeAttack", _hitPoint);

	

		
	}

	// DEBUG : DrawCapsule
	DrawDebugCapsule(GetWorld(), center, attackRange*0.5f, attackRadius, quat, drawColor, false, 2.0f);
}





void AMyPlayer::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		_varical = MovementVector.Y;
		_horizontal = MovementVector.X;

		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AMyPlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);

	}
}

void AMyPlayer::JumpA(const FInputActionValue& Value)
{
	bool isPressed = Value.Get<bool>();

	if (isPressed)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Jump!!"));
		ACharacter::Jump();
	}
}

void AMyPlayer::AttackA(const FInputActionValue& Value)
{
	bool isPressed = Value.Get<bool>();

	UE_LOG(LogTemp, Log, TEXT("Attack"));
	if (isPressed && _isAttacking == false && _PianimInstance != nullptr)
	{


		_PianimInstance->PlayAttackMontage();
		_isAttacking = true;

		_curAttackIndex %= 3;
		_curAttackIndex++;


		_PianimInstance->JumpToSection(_curAttackIndex);
	}


}

void AMyPlayer::Death(const FInputActionValue& Value)
{
	bool isPressed = Value.Get<bool>();

	if (isPressed && _isAttacking == false && _PianimInstance != nullptr)
	{
		_PianimInstance->PlayDeath();

		_isAttacking = true;
	}
}

void AMyPlayer::Drop(const FInputActionValue& Value)
{
	bool isPressed = Value.Get<bool>();
	if (isPressed)
	{
		DropItem();
	}
}

