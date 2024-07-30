// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "MyAnimInstance.h"
#include "Engine/DamageEvents.h"
#include "MyItem.h"

// Sets default values

AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//TODO
	//skeletal Mash
	// /Script/Engine.SkeletalMesh'/Game/ParagonKallari/Characters/Heroes/Kallari/Meshes/Kallari.Kallari'
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> sm
	(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonKallari/Characters/Heroes/Kallari/Meshes/Kallari.Kallari'"));

	if (sm.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(sm.Object);
	}
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));

	_springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	_camera = CreateDefaultSubobject< UCameraComponent>(TEXT("Camera"));

	// 상속관계 설정 
	_springArm->SetupAttachment(GetCapsuleComponent());
	_camera->SetupAttachment(_springArm);

	_springArm->TargetArmLength = 500.0f;
	_springArm->SetRelativeRotation(FRotator(-35.0f, 0.0f, 0.0f));

	/*MaxHP = 100;
	CurHP = MaxHP;*/
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	Init();

}

void AMyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	_animInstance = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());
	// 몽타주가 끝날 때 _isAttack 을 false로 만들어줬으면 좋겠다.
	if(_animInstance->IsValidLowLevelFast())
	{ 
	_animInstance->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnAttackEnded);
	_animInstance->_attackDelegate.AddUObject(this, &AMyCharacter::Attackhit);
	_animInstance->_deathDelegate.AddUObject(this, &AMyCharacter::Disable);
	}
	
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//_myDelegate3.ExecuteIfBound(50,30);
	// hp: 50, mp : 30
	//_myDelegate2.Execute();

	// 클래스 객체로 직접 함수 호출 
	/*auto myAnimI = GetMesh()->GetAnimInstance();
	Cast<UMyAnimInstance>(myAnimI)->DelegateTest2(50, 30);*/
	// 단점 클래스간의 결합도가 높아짐 
}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Moving
		EnhancedInputComponent->BindAction(_moveAction, ETriggerEvent::Triggered, this, &AMyCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(_lookAction, ETriggerEvent::Triggered, this, &AMyCharacter::Look);
	
		// jumping
		EnhancedInputComponent->BindAction(_jumpAction, ETriggerEvent::Triggered, this, &AMyCharacter::JumpA);
	
		// Attack
		EnhancedInputComponent->BindAction(_attackAction, ETriggerEvent::Started, this, &AMyCharacter::AttackA);
		
		// chuck
		EnhancedInputComponent->BindAction(_itemDrop, ETriggerEvent::Triggered, this, &AMyCharacter::Drop);

	}
}

float AMyCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{

	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	// TODO 
	// 1.  hp -= Damage
	// 2. 공격자 이름 출력
	// TODO 
	//FDamageEvent DamageEvent;
	_curHp -= Damage;
	UE_LOG(LogTemp, Log, TEXT("Attack : %s, CurHp :%d"), *DamageCauser->GetName(), _curHp);
	if (_curHp <= 0)
	{
		_curHp = 0;
		/*_animInstance->PlayDeath();*/
		//_isAttacking = true;  
		
		//SetLifeSpan(5.0f);
		
			ItempDrop();
		
		
	}
	return _curHp;

	//int Damgea = Damage; // 내가한부분
	//CurHP -= Damgea;
	//UE_LOG(LogTemp, Warning, TEXT("Hit Acttor NAME: %s Damage: %d"), *GetName(), Damgea);
	//

	//if (CurHP <= 0)
	//{
	//	CurHP = 0;
	//	UE_LOG(LogTemp, Warning, TEXT("%s is dead"), *GetName());
	//	Destroy();
	//}

	//return Damgea;
}

void AMyCharacter::OnAttackEnded(UAnimMontage* Montage, bool bInterrupted)
{
	//UE_LOG(LogTemp, Error, TEXT("Attack!! END!!"));
	_isAttacking = false;
}

void AMyCharacter::Attackhit()
{
	//auto* AnimI = GetMesh()->GetAnimInstance();
	//Cast<UMyAnimInstance>(AnimI)->AnimNotify_Attackhit();
	// TODO : Attack  충돌처리
	// 1. 히트스캔으로 공격하기, AttackRange는 본인 마음대로
	// 2. DebugDraw 까지
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

		// TODO : TakeDamage
		FDamageEvent DamageEvent;
		hitResult.GetActor()->TakeDamage(_attackDamage, DamageEvent, GetController(), this);
		//TakeDamage(10.0f, , )
		//UE_LOG(LogTemp, Warning, TEXT("Hit Acttor NAME: %s"), hitResult.GetActor->GetName());
		
		// 내가한부분
		//FDamageEvent DamageEvent;
		//hitResult.GetActor()->TakeDamage(20, DamageEvent, GetController(), this);
	}

	DrawDebugSphere(GetWorld(), center, attackRadius, 12, drawColor, false, 2.0f);
}




void AMyCharacter::AddItem(AMyItem* item)
{

	// Add


}

void AMyCharacter::DropItem()
{
	//Drop


}

void AMyCharacter::Move(const FInputActionValue& Value)
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

void AMyCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
	
	}
}

void AMyCharacter::JumpA(const FInputActionValue& Value)
{
	bool isPressed = Value.Get<bool>();
	
	if(isPressed)
	{
	//UE_LOG(LogTemp, Warning, TEXT("Jump!!"));
		ACharacter::Jump();
	}
}

void AMyCharacter::AttackA(const FInputActionValue& Value)
{
	bool isPressed = Value.Get<bool>();
	
	if (isPressed && _isAttacking == false && _animInstance != nullptr)
	{
	
		//UE_LOG(LogTemp, Warning, TEXT("Attack!!"));

		_animInstance->PlayAttackMontage();
		_isAttacking = true;

		_curAttackIndex %= 3;
		_curAttackIndex++;


		_animInstance->JumpToSection(_curAttackIndex);
	}


}

void AMyCharacter::Death(const FInputActionValue& Value)
{
	bool isPressed = Value.Get<bool>();

	if (isPressed && _isAttacking == false && _animInstance != nullptr)
	{
		_animInstance->PlayDeath();
		_isAttacking = true;
	}
}

void AMyCharacter::Drop(const FInputActionValue& Value)
{
	bool isPressed = Value.Get<bool>();
	if (isPressed)
	{
		//RemoveFromInventory(ItemDrop);
		
		
		ItempDrop();
		 
		/*	ItemToDrop->SetActorLocation(DropLocation);*/
		//ItemDrop->SetActorHiddenInGame(false);
		//ItemDrop->SetActorEnableCollision(true);
	}

}

void AMyCharacter::Init()
{
	_curHp = _maxHp;
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	PrimaryActorTick.bCanEverTick = true;

}

void AMyCharacter::Disable()
{
	// 비활성화 
	SetActorHiddenInGame(true); 
	SetActorEnableCollision(false);
	PrimaryActorTick.bCanEverTick = false;

}

void AMyCharacter::InvenAdd(AMyItem* Item)
{
	if (Item && !Inven.Contains(Item))
	{
		Inven.Add(Item);
	}

}

void AMyCharacter::ItempDrop()
{
	if (Inven.Num() > 0)
	{
		/*AMyItem* ItemDrop = Inven.Last();
		Inven.Remove(ItemDrop);
		
		FVector DropLocation = GetActorLocation() + GetActorForwardVector() * 500.0f;
		
		UE_LOG(LogTemp, Warning, TEXT("Drop Item!!"));
		ItemDrop->SetActorLocation(DropLocation);
		
		ItemDrop->SetActorHiddenInGame(false);
		ItemDrop->SetActorEnableCollision(true);*/

	

		TArray<class AMyItem*> ItemDrop = Inven;

		for (auto item : ItemDrop)
		{
			if (item)
			{
				Inven.Remove(item);

				FVector CharacterLocation =GetActorLocation();
				FVector RandomOffset = FVector(FMath::FRandRange(-200.0f, 200.0f), FMath::FRandRange(-200.0f, 200.0f), 50);

				FVector DropLocation = CharacterLocation + RandomOffset;

				UE_LOG(LogTemp, Warning, TEXT("Drop Item!!"));
				item->SetActorLocation(DropLocation);

				item->SetActorHiddenInGame(false);
				item->SetActorEnableCollision(true);

			}
		}
	}

}
