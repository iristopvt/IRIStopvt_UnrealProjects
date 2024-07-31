// Fill out your copyright notice in the Description page of Project Settings.


#include "MyItem.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "MyCharacter.h"

// Sets default values
AMyItem::AMyItem()
{
	PrimaryActorTick.bCanEverTick = false;

	_meshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	_trigger = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> sn
	(TEXT("/Script/Engine.StaticMesh'/Game/Weapon_Pack/Mesh/Weapons/Weapons_Kit/SM_Sword.SM_Sword'"));


	if (sn.Succeeded())
	{
		_meshComponent->SetStaticMesh(sn.Object);
	}

	_meshComponent->SetupAttachment(RootComponent);
	_trigger->SetupAttachment(_meshComponent);

	_meshComponent->SetCollisionProfileName(TEXT("MyItem"));
	_trigger->SetCollisionProfileName(TEXT("MyItem"));
	_trigger->SetSphereRadius(60.0f);

	//_trigger->OnComponentBeginOverlap.Add
	
}

// Called when the game starts or when spawned
void AMyItem::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMyItem::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	_trigger->OnComponentBeginOverlap.AddDynamic(this, &AMyItem::OnMyCharacteroverlap);
	
	

}

void AMyItem::OnMyCharacteroverlap(UPrimitiveComponent* OverlappedComponent, AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool BFromWeep, const FHitResult& SweepResult)
{
	auto myCharacter = Cast<AMyCharacter>(otherActor);
	if (myCharacter)
	{	
		UE_LOG(LogTemp, Log, TEXT("%s collision"), *myCharacter->GetName());
		//Inven.Add(sn);
		// myCharacter->_attackDamage += 10;
		myCharacter->AddAttackDamage(this,50);
		UE_LOG(LogTemp, Warning, TEXT("Damage UP!!"));
		myCharacter->AddItem(this);
		// myCharacter->InvenAdd(this);
		
		Disable();
	}
}

void AMyItem::Init()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
}

void AMyItem::Disable()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}

// Called every frame
void AMyItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyItem::SetItemPos(FVector pos, FRotator rot)
{
	Init();
	SetActorLocation(pos);
	SetActorRotation(rot);
}


