// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"

#include "EnhancedInputComponent.h"

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* subSystem = 
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (subSystem)
	{
		subSystem->AddMappingContext(_inputMappingContext, 0);
	}



}
void AMyPlayerController::OpenInventory()
{
}

void AMyPlayerController::CloseInventory()
{
}

void AMyPlayerController::SetupInputComponent()
{

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		//EnhancedInputComponent->BindAction(InvenOpenAction, ETriggerEvent::Triggered, this, &AMyPlayerController::HandleInvenOpen);
	}
}



