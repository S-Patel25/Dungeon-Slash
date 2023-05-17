// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthBar.generated.h"

/**
 * 
 */

class UProgressBar;

UCLASS()
class HSGAME_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()
	


public:

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthBar; //to bind cpp varibles to widget, make sure name is EXACTLY same as widget variable name, and use bindwidget specifier

};
