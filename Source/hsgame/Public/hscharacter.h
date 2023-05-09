// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h" //sometimes its okay to include small header files
#include "CharacterTypesEnum.h"

//always make sure this is the last header file
#include "hscharacter.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class AMyActor;
class UAnimMontage;
class AWeaponClass;

UCLASS()
class HSGAME_API Ahscharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	Ahscharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable)
	void setBoxCollision(ECollisionEnabled::Type collisionEnabled);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	AWeaponClass* equippedWeapon;

	//inputs

	UPROPERTY(EditAnywhere, Category = "Input") //DONT FORGET TO ADD UPROPERTY
	UInputMappingContext* context;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* moveForward;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* look;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* jumpAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* equipItemWithE;

	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* attack;

	void MoveForward(const FInputActionValue& value);

	void Look(const FInputActionValue& value);

	void equipItem();

	void Attack();

	//montages

	UPROPERTY(EditDefaultsOnly, Category = "Montages") //make sure its exposed to ue's reflection system
	UAnimMontage* attackMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montages") //multiple montages for different behaviours
	UAnimMontage* equipMontage;

	void playAttackMontage();

	void playEquipMontage(const FName& sectionName); //play equip montage based on state

	UFUNCTION(BlueprintCallable)
	void AttackEndNotify();

	UFUNCTION(BlueprintCallable)
	void UnequipNotify();

	UFUNCTION(BlueprintCallable)
	void EquipNotify();

	UFUNCTION(BlueprintCallable)
	void FinishEquipOrUnequip();


	bool CanAttack();
	bool CanDisarm();
	bool CanArm();

private:

	ECharacterState characterState = ECharacterState::ECS_Unequipped; //setting default value for enum (unequipped as player should not start with a weapon)

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true")) //putting specifier so we can access state in anim bp
	EActionState actionState = EActionState::EAS_Unoccupied;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* springArm;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* camera;

	UPROPERTY(VisibleAnywhere)
	AMyActor* OverlappingItem;


public:
	FORCEINLINE void SetOverlappingItem(AMyActor* item) { OverlappingItem = item; } //using getters and setters to get overlapping actor, we use inline to make it more optimized, typically used for getters and setters and/or very small code
	FORCEINLINE ECharacterState SetCharacterState() const { return characterState; } //since this is just a var, no need to have any parameters, just return the type
	FORCEINLINE EActionState SetActionState() const { return actionState; }
};
