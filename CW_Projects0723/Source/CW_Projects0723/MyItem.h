// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyItem.generated.h"

//UENUM()
//enum class ItemType
//{
//
//
//}
//
//USTRUCT()
//struct ItenInfo
//{
//	int itemId;
//	ItemType Type;
//
//};



UCLASS()
class CW_PROJECTS0723_API AMyItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnMyCharacteroverlap(UPrimitiveComponent* OverlappedComponent,AActor* otherActor, UPrimitiveComponent* otherComp, int32 otherBodyIndex, bool BFromWeep, const FHitResult& SweepResult );

public:	
	void Disable();
	void Init();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void SetItemPos(FVector pos, FRotator rot = FRotator::ZeroRotator);

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* _meshComponent;

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* _trigger;
	
	//UFUNCTION()
	//TArray<AMyItem*> Inven;

	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 _itemId = -1;

	/*UPROPERTY(VisibleAnywhere)
	ItenInfo _info;*/

};
