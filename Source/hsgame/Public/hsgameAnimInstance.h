// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterTypesEnum.h" //we can use this anywhere now since we made custom header file for enum types for character
#include "hsgameAnimInstance.generated.h"

class Ahscharacter;
class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class HSGAME_API UhsgameAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeInitializeAnimation() override; //equivalent of the two functions from the anim bp

	virtual void NativeUpdateAnimation(float DeltaTime) override; //overriding base functions from super so we can use them

	UPROPERTY(BlueprintReadOnly) //bpreadonly exposes to the event graph
	Ahscharacter* character;
	
	UPROPERTY(BlueprintReadOnly)
	UCharacterMovementComponent* moveComponent;

	UPROPERTY(BlueprintReadOnly)
	float GroundSpeed; //how fast character is going


	UPROPERTY(BlueprintReadOnly) //check to see if player is falling so we can play correct animation
	bool isFalling;

	UPROPERTY(BlueprintReadOnly) //makinmg this bpreadonly so we can access in it our animation blueprint
	ECharacterState characterState;
};
