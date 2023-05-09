// Fill out your copyright notice in the Description page of Project Settings.


#include "hsgameAnimInstance.h"
#include "hscharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"



void UhsgameAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	character = Cast<Ahscharacter>(TryGetPawnOwner()); //this is the equivalent of doing the try get pawn owner and casting to our character bp in blueprints 

	if (character)
	{
		moveComponent = character->GetCharacterMovement(); //getting the char movement component of our character and assigning it to the move comp variable we created
	}

}

void UhsgameAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (moveComponent)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(moveComponent->Velocity); //set ground speed to the magnitude of our vector (since kismet is static, thats why we use the double colon)
		isFalling = moveComponent->IsFalling();
		characterState = character->SetCharacterState();
	}
}
