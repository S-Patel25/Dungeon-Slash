// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyActor.h"
#include "Treasure.generated.h"

/**
 * 
 */
UCLASS()
class HSGAME_API ATreasure : public AMyActor
{
	GENERATED_BODY()

protected:

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundBase* pickupSound;

	UPROPERTY(EditAnywhere, Category = "Gold")
	int32 goldCount;

};
