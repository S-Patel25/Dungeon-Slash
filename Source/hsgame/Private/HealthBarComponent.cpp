// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBarComponent.h"
#include "HealthBar.h"
#include "Components/ProgressBar.h"

void UHealthBarComponent::SetHealthPercent(float percent)
{

	if (healthBarWidget == nullptr) //make member variable as casting can be expensive operation
	{
		healthBarWidget = Cast<UHealthBar>(GetUserWidgetObject()); //cast to healthbar class
	}

	if (healthBarWidget && healthBarWidget->HealthBar) //nullptr check
	{
		healthBarWidget->HealthBar->SetPercent(percent); //use pointer to access health bar var to set its percentage
	}


}
