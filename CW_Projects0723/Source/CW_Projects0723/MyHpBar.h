// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyHpBar.generated.h"

/**
 * 
 */
UCLASS()
class CW_PROJECTS0723_API UMyHpBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetHpBarvalue(float ratio);
	
private:
	//UPROPERTY(mata = (BindWidget))
	//class UProgressBar* PB_HPBar;
	UPROPERTY(meta = (BindWidget))
	class UProgressBar* PB_HPBar;
};
