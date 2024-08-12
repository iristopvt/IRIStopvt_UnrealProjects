// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
// UI
#include "MyGameInstance.h"
#include "MyUIManager.h"
#include "MyInventoryUI.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "MyAnimInstance.h"
#include "Engine/DamageEvents.h"
#include "Math/UnrealMathUtility.h"
#include "MyItem.h"
#include "MyStatComponent.h"
#include "MyInvenComponent.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "MyHpBar.h"
#include "MyPlayerController.h"
#include "Components/Button.h"
#include "MyAIController.h"


// _particle
#include "MyEffecManger.h"


// Sets default values

AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> sm
	(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonGreystone/FX/Particles/Greystone/Abilities/Deflect/FX/P_Greystone_Deflect_Remove.P_Greystone_Deflect_Remove'"));

	if (sm.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(sm.Object);
	}
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));

	RootComponent = GetCapsuleComponent();

	//stat
	_statCom = CreateDefaultSubobject< UMyStatComponent>(TEXT("Stat"));
	//_statCom->_deathDelegate.AddLambda([this]()-> void { this->GetController()->UnPossess(); });
	// 위에부분 추가 

	// 도저히 모르겠어서 gpt 물어봄 
	if (_statCom)
	{
		// Ensure _statCom is valid before adding the lambda
		_statCom->_deathDelegate.AddLambda([this]()
			{
				if (AController* Controller = this->GetController())
				{
					Controller->UnPossess();
				}
			});
	}
	//

	_invenCom = CreateDefaultSubobject<UMyInvenComponent>(TEXT("Inven TO"));

	_hpbarkwidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HpBar"));
	_hpbarkwidget->SetupAttachment(GetMesh());
	_hpbarkwidget->SetWidgetSpace(EWidgetSpace::Screen);
	_hpbarkwidget->SetRelativeLocation(FVector(0.0f, 0.0f, 220.0f));

	static ConstructorHelpers::FClassFinder<UUserWidget> hpBar(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/BulePrint/UI/MyHpBar_BP.MyHpBar_BP_C'"));

	if (hpBar.Succeeded())
	{
		_hpbarkwidget->SetWidgetClass(hpBar.Class);
	}

	APawn::AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;


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


float AMyCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{

	Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	
	float damaged = -_statCom->AddCurHp(-Damage);

	if (_statCom->IsDead())
	{
		/*if(_invenCom != nullptr)
			_invenCom->AllDropItem();*/
		//Disable();
	}
	
	return damaged;
}

void AMyCharacter::OnAttackEnded(UAnimMontage* Montage, bool bInterrupted)
{
	_isAttacking = false;
	_attackEndendDelegate.Broadcast();
}

void AMyCharacter::Attackhit()
{
	
	//FHitResult hitResult;
	//FCollisionQueryParams params(NAME_None, false, this);

	//float attackRange = 500.0f;
	//float attackRadius = 150.0f;
	//FVector forward = GetActorForwardVector();

	//FQuat quat = FQuat::FindBetweenVectors(FVector(0, 0, 1), forward); // FRotationMatrix::MakeFromZ(forward).ToQuat();
	//
	//bool bResult = GetWorld()->SweepSingleByChannel
	//(
	//	hitResult,
	//	GetActorLocation(),
	//	GetActorLocation() + GetActorForwardVector() * attackRange,
	//	quat,
	//	ECollisionChannel::ECC_GameTraceChannel2,
	//	FCollisionShape::MakeSphere(attackRadius),
	//	params
	//	);

	//FVector vec = GetActorForwardVector() * attackRange;
	//FVector center = GetActorLocation() + vec * 0.5f;
	//
	//FColor drawColor = FColor::Green;


	//if (bResult && hitResult.GetActor()->IsValidLowLevel())
	//{
	//	drawColor = FColor::Red;

	//	FDamageEvent DamageEvent;
	//	hitResult.GetActor()->TakeDamage(_statCom->GetAttackDamage(), DamageEvent, GetController(), this);
	//	
	//}
	//
	//DrawDebugSphere(GetWorld(), center, attackRadius, 12, drawColor, false, 2.0f);
	FHitResult hitResult;
	FCollisionQueryParams params(NAME_None, false, this);

	float attackRange = 1000.0f;
	float attackRadius = 20.0f;

	FVector forward = GetActorForwardVector();
	FQuat quat = FQuat::FindBetweenVectors(FVector(0, 0, 1), forward); // FRotationMatrix::MakeFromZ(forward).ToQuat();

	FVector center = GetActorLocation() + forward * attackRange * 0.5f;
	FVector start = GetActorLocation();
	FVector end = GetActorLocation() + forward * (attackRange * 0.5f);

	bool bResult = GetWorld()->SweepSingleByChannel
	(
		hitResult,
		start,
		end,
		quat,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeCapsule(attackRadius, attackRange * 0.5f),
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

		//EffecManger->Play("MeleeAttack", _hitPoint);
		//_attackHitEvent.Broadcast();
	
	}


	// DEBUG : DrawCapsule
	DrawDebugCapsule(GetWorld(), center, attackRange * 0.5f, attackRadius, quat, drawColor, false, 2.0f);
}



void AMyCharacter::AddAttackDamage(AActor* actor, int amount)
{
	//Actor는 나의 공격력을 버프해준 대상 
	_statCom->AddAttackDamage(amount);;
}

void AMyCharacter::AddItemToCharacter(AMyItem* item)
{
	_invenCom->AddItem(item);
}

void AMyCharacter::DropItem()
{
	_invenCom->DropItem();
}



//
//void AMyCharacter::AddItem(AMyItem* item)
//{
//	// Add
//	if(_item != nullptr)
//		_item->AddItem(item);
//	//UE_LOG(LogTemp, Log, TEXT("ItemSize : %d"), _item->_inven.Num());
//}
//
//void AMyCharacter::DropItem()
//{
//	//Drop
//	if(_item != nullptr)
//		_item->DropItem();
//	
//}



void AMyCharacter::Init()
{
	_statCom->Reset();
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	PrimaryActorTick.bCanEverTick = true;
	_statCom->_deathDelegate.AddLambda([this]()-> void { this->GetController()->UnPossess(); }); // 기존 강사님 부분 

	if (_aiController && GetController() == nullptr) // 컨트롤러를 다시 입혀햐하기때문에 컨트롤러로 기억할수있도록... 재 빙의를 위해...
	{
		auto ai_Controller = Cast<AMyAIController>(_aiController);
		if (ai_Controller)
			ai_Controller->Possess(this);
	}
}

void AMyCharacter::Disable()
{
	// ��Ȱ��ȭ 
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	PrimaryActorTick.bCanEverTick = false;
	auto controller = GetController();
	if (controller)
		GetController()->UnPossess();
	//UnPossessed();
 // 위에 한줄 추가 
}

