// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h" //sometimes its okay to include small header files
#include "birdPawn.generated.h"


class UCapsuleComponent; //good to forward declare at the top
class USkeletalMeshComponent;
class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;


UCLASS()
class HSGAME_API AbirdPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AbirdPawn();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* birdContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* input; //once u make IA, then you have to make method corresponding to said IA

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* look;

	void Move(const FInputActionValue& value); //this is same thing as value from old way, except new val can store bool, 2d vector, float, vector. (& means reference)

	void Look(const FInputActionValue& value);


private:

	//Remember, cant use blueprintreadwrite on private vars, only with meta keyword

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* capsule;
	//class keyword before var is known as forward declaration (used for memory allocation and optimazition, and it will be complete in the cpp file as that is where the header is, bascially saying use only when needed)
	//once a class has been forward declared, it CAN be used again without needing the class keyword, so long as its below the original declaration

	UPROPERTY(VisibleAnywhere) 
	USkeletalMeshComponent* sMesh;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* springArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* camera;


};
