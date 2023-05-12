// Fill out your copyright notice in the Description page of Project Settings.


#include "AttributeComponent.h"

UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UAttributeComponent::tookDamage(float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0.f, maxHealth); //using clamp so health never goes below 0



}

float UAttributeComponent::getHealthPercent()
{
	return Health / maxHealth; //simple getter function for getting the health
}

bool UAttributeComponent::isAlive()
{
	return Health > 0.f; //simple bool to check to see if enemy is alive
}


// Called every frame
void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

