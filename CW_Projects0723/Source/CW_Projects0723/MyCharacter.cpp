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
#include "Math/UnrealMath.h"
#include "MyStatComponent.h"
#include "MyInvenComponent.h"
#include "Components/WidgetComponent.h"
#include "MyHpBar.h"
#include "MyInventoryUI.h"
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

	_springArm->SetupAttachment(GetCapsuleComponent());
	_camera->SetupAttachment(_springArm);

	_springArm->TargetArmLength = 500.0f;
	_springArm->SetRelativeRotation(FRotator(-35.0f, 0.0f, 0.0f));

	//stat
	_statCom = CreateDefaultSubobject< UMyStatComponent>(TEXT("Stat"));
	_item = CreateDefaultSubobject<UMyInvenComponent>(TEXT("Inven"));
	
	_hpbarkwidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HpBar"));
	_hpbarkwidget->SetupAttachment(GetMesh());
	_hpbarkwidget->SetWidgetSpace(EWidgetSpace::Screen);
	_hpbarkwidget->SetRelativeLocation(FVector(0.0f, 0.0f, 220.0f));

	static ConstructorHelpers::FClassFinder<UUserWidget> hpBar(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BulePrint/UI/MyHpBar_BP.MyHpBar_BP_C'"));

	if (hpBar.Succeeded())
	{
		_hpbarkwidget->SetWidgetClass(hpBar.Class);
	}


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
	if(_animInstance->IsValidLowLevelFast())
	{ 
	_animInstance->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnAttackEnded);
	_animInstance->_attackDelegate.AddUObject(this, &AMyCharacter::Attackhit);
	_animInstance->_deathDelegate.AddUObject(this, &AMyCharacter::Disable);
	}
	
	_statCom->SetLevellAndInit(_level);
	//_statCom->_hpChangedDelegate.Add()
	_hpbarkwidget->InitWidget();
	auto hpBar = Cast<UMyHpBar>(_hpbarkwidget->GetUserWidgetObject());
	
	if (hpBar)
	{
		_statCom->_hpChangedDelegate.AddUObject(hpBar, &UMyHpBar::SetHpBarvalue);
	}
	

}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
		EnhancedInputComponent->BindAction(_itemDropAction, ETriggerEvent::Triggered, this, &AMyCharacter::DropItem);

		// Invenopen
		//EnhancedInputComponent->BindAction(_InvenOpenAction, ETriggerEvent::Triggered, this, &AMyCharacter::InventoryOpen);

	}
}

float AMyCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{

	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	
	float damaged = -_statCom->AddCurHp(-Damage);

	if (_statCom->IsDead())
	{
		if(_item != nullptr)
			_item->AllDropItem();

	}
	
	return damaged;
}

void AMyCharacter::OnAttackEnded(UAnimMontage* Montage, bool bInterrupted)
{
	_isAttacking = false;
}

void AMyCharacter::Attackhit()
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

void AMyCharacter::AddAttackDamage(AActor* actor, int amount)
{
	//Actor는 나의 공격력을 버프해준 대상 
	_statCom->AddAttackDamage(amount);;
}




void AMyCharacter::AddItem(AMyItem* item)
{
	// Add
	if(_item != nullptr)
		_item->AddInven(item);
	//UE_LOG(LogTemp, Log, TEXT("ItemSize : %d"), _item->_inven.Num());
}

void AMyCharacter::DropItem()
{
	//Drop
	if(_item != nullptr)
		_item->DropItem();
	
}

void AMyCharacter::InventoryOpen()
{
	UE_LOG(LogTemp, Log, TEXT("Inven OPEN!!"));
	/*_myInventory = CreateDefaultSubobject<UWidgetComponent>(TEXT("InvenTorty"));
	_myInventory->SetupAttachment(GetMesh());
	_myInventory->SetWidgetSpace(EWidgetSpace::Screen);
	_myInventory->SetRelativeLocation(FVector(0.0f, 0.0f, 110.0f));
	

	static ConstructorHelpers::FClassFinder<UUserWidget> myInopen(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BulePrint/UI/MyInventoryUI_BP.MyInventoryUI_BP_C'"));

	if (myInopen.Succeeded())
	{
		_myInventory->SetWidgetClass(myInopen.Class);
	}*/
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

void AMyCharacter::InOpen(const FInputActionValue& Value)
{
	bool isPressed = Value.Get<bool>();
	if (isPressed)
	{
		InventoryOpen();
	}

}

void AMyCharacter::Drop(const FInputActionValue& Value)
{
	bool isPressed = Value.Get<bool>();
	if (isPressed)
	{
		DropItem();
	}

}

void AMyCharacter::Init()
{
	_statCom->Reset();
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	PrimaryActorTick.bCanEverTick = true;

}

void AMyCharacter::Disable()
{
	// ��Ȱ��ȭ 
	SetActorHiddenInGame(true); 
	SetActorEnableCollision(false);
	PrimaryActorTick.bCanEverTick = false;

}

