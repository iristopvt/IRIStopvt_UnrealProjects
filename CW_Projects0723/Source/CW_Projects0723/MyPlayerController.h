// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"


/**
 * 
 */
class UInputComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

UCLASS()
class CW_PROJECTS0723_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()


public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* _lookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* _moveAction;



protected:
	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = Input)
	class UInputMappingContext* _inputMappingContext;


	virtual void BeginPlay() override;

protected:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void UpDown(float value);
	void RightLeft(float value);

};
