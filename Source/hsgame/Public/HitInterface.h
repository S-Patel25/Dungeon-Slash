// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HitInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHitInterface : public UInterface //this is for reflection system
{
	GENERATED_BODY()
};

/**
 * 
 */
class HSGAME_API IHitInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintNativeEvent) //this allows class to be used in BP's, AND cpp classes
	void GetHit(const FVector& impactPoint); // using vector to see where enemy got hit

};
