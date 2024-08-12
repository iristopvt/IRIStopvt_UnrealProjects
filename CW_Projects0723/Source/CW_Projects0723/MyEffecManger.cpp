// Fill out your copyright notice in the Description page of Project Settings.


#include "MyEffecManger.h"

#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

// Sets default values
AMyEffecManger::AMyEffecManger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CreateParticle("MeleeAttack", "/Script/Engine.ParticleSystem'/Game/ParagonGreystone/FX/Particles/Greystone/Skins/Novaborn/P_Greystone_Novaborn_SwordSpinning_01.P_Greystone_Novaborn_SwordSpinning_01'");
}

void AMyEffecManger::CreateParticle(FString name, FString path)
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem> explosion(*path);
	{
		if (explosion.Succeeded())
		{
				UParticleSystem* particle = explosion.Object;

				_table.Add(name, particle);

		}
	}
}

// Called when the game starts or when spawned
void AMyEffecManger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyEffecManger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyEffecManger::Play(FString name, FVector location, FRotator rotator)
{
	if (_table.Contains(name) == false)
		return;

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), _table[name], location, rotator);
	//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleSystem, location, rotator);


}

