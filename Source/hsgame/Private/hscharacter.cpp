// Fill out your copyright notice in the Description page of Project Settings.


#include "hscharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyActor.h"
#include "WeaponClass.h"
#include "Animation/AnimMontage.h"
#include "Components/BoxComponent.h"

// Sets default values
Ahscharacter::Ahscharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;  //set these to false to eliminate that weird behaviour
	bUseControllerRotationPitch = false;
	bUseControllerRotationPitch = false;

	GetCharacterMovement()->bOrientRotationToMovement = true; //move character based on the rotation (where the mouse position is)

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	springArm->SetupAttachment(GetRootComponent());
	springArm->TargetArmLength = 300.f;

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(springArm);

}

// Called when the game starts or when spawned
void Ahscharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* controller = Cast<APlayerController>(GetController()); //equivalent of get controller, and cast to player controller in blueprints

	if (controller) //null check
	{
		//neat trick if want to use is putting var declaration inside if statement scope so thats its only valid in if statment, can't be used outside
		UEnhancedInputLocalPlayerSubsystem* subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(controller->GetLocalPlayer()); //equivalent of getting subsystem directly in blueprints, getlocalplayer is done automatically in bp, in cpp we do it manually

		if (subSystem)
		{
			subSystem->AddMappingContext(context, 0); //final step of adding it to the pawn, NOW all intializing has been done
		}
	}

	
}

void Ahscharacter::MoveForward(const FInputActionValue& value)
{
	const FVector2D vector = value.Get<FVector2D>();

	//if (Controller)
	//{
	//	const FVector forward = GetActorForwardVector(); //means up and down (y-axis)
	//	AddMovementInput(forward, vector.Y);

	//	const FVector right = GetActorRightVector(); // means left and right (x-axis)
	//	AddMovementInput(right, vector.X);
	//}

	//Since we want the character to move in the direction of the rotation (mouse), we use something a rotation matrix which takes in the direction, and returns a normalized vector (unit vector), from that vector we can see the direction and move from there.

	if (actionState != EActionState::EAS_Unoccupied) return;

	const FRotator rotation = Controller->GetControlRotation(); //gets rotation of the controller 
	const FRotator yawRotation(0.f, rotation.Yaw, 0.f); 

	const FVector forwardDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X); //gets the x axis vector of the rotation matrix dot product
	AddMovementInput(forwardDirection, vector.Y);

	const FVector rightDirection = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y); //gets the y axis vector of the rotation matrix dot product
	AddMovementInput(rightDirection, vector.X); //now that we have the vector of our current rotation, we can move accordingly
}

void Ahscharacter::Look(const FInputActionValue& value)
{
	const FVector2D vector = value.Get<FVector2d>();

	if (Controller)
	{
		AddControllerYawInput(vector.X); //basic mouse look around input using built in functions
		AddControllerPitchInput(vector.Y);
	}

}

void Ahscharacter::equipItem()
{
	AWeaponClass* overlappingWeapon = Cast<AWeaponClass>(OverlappingItem); 

	if (overlappingWeapon)
	{
		if (overlappingWeapon->ActorHasTag(FName("OneHanded")))
		{
			overlappingWeapon->Equip(GetMesh(), FName("RightHandSocketOneHanded"), this, this); //now that we've cast, we can equip the weapon now
		}
		else if (overlappingWeapon->ActorHasTag(FName("TwoHanded")))
		{
			overlappingWeapon->Equip(GetMesh(), FName("RightHandSocketTwoHanded"), this, this);
		}

		OverlappingItem = nullptr;
		equippedWeapon = overlappingWeapon; //setting the weapon to variable to keep track

		if (equippedWeapon->ActorHasTag(FName("OneHanded"))) //check to see what kind of weapon user has, so correct animations can be played
		{
			characterState = ECharacterState::ECS_EquippedOneHandWeapon;
		}
		else if (equippedWeapon->ActorHasTag(FName("TwoHanded"))) //using actor tags to determine the type of weapon player is holding, so enum state can be changed based on that
		{
			characterState = ECharacterState::ECS_EquippedTwoHandWeapon;
		}

	}
	else //if we're not over;lapping the weapon (means we either have it or not in the overlap zone, then we do other checks)
	{
		if (CanDisarm())
		{
			playEquipMontage(FName("Unequip"));
			characterState = ECharacterState::ECS_Unequipped; //setting the state back to unequipped as player has disarmed weapon
			actionState = EActionState::EAS_EquippingOrUnequippingWeapon;
		}
		else if (CanArm())
		{
			playEquipMontage(FName("Equip"));

			if (equippedWeapon->ActorHasTag(FName("OneHanded")))
			{
				characterState = ECharacterState::ECS_EquippedOneHandWeapon;
			}
			else if (equippedWeapon->ActorHasTag(FName("TwoHanded")))
			{
				characterState = ECharacterState::ECS_EquippedTwoHandWeapon;
			}
			actionState = EActionState::EAS_EquippingOrUnequippingWeapon;
		}
	}

}

void Ahscharacter::Attack()
{
	if (CanAttack()) //checks to see if player is holding weapon AND if they are unoccupied
	{
		playAttackMontage();
		actionState = EActionState::EAS_Attacking;
	}
	

}

bool Ahscharacter::CanAttack() //simple bool check to see if player can attack
{
	return actionState == EActionState::EAS_Unoccupied && 
		characterState != ECharacterState::ECS_Unequipped;
}

bool Ahscharacter::CanDisarm() //checks to see if player has a weapon and not in a current action state
{
	return actionState == EActionState::EAS_Unoccupied &&
		characterState != ECharacterState::ECS_Unequipped;
}

bool Ahscharacter::CanArm()
{
	return actionState == EActionState::EAS_Unoccupied &&
		characterState == ECharacterState::ECS_Unequipped &&
		equippedWeapon; //also check to see if weapon exists
}

void Ahscharacter::playAttackMontage()
{
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance(); //getting the anim instance of our character 

	if (animInstance && attackMontage) //always check for nullptr (also checking to see if our attack montage also exists)
	{
		animInstance->Montage_Play(attackMontage); //equivalent of dragging node off mesh and searching for play montage in bp's

		int32 randAttack = FMath::RandRange(0, 2); //code below basically takes a random number from 0-2 (as there are 3 attacks), and selects one random to play using the montage jump to section (so the attacks are varied and not the same, can change later)
		FName sectionName = FName();

		switch (randAttack)
		{
		case 0:
			sectionName = FName("Attack1"); //setting the section names based on rand number
			break;
		case 1:
			sectionName = FName("Attack2");
			break;
		case 2:
			sectionName = FName("Attack3");
			break;
		}

		animInstance->Montage_JumpToSection(sectionName, attackMontage);
	}

}

void Ahscharacter::playEquipMontage(const FName& sectionName)
{
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();

	if (animInstance && equipMontage)
	{
		animInstance->Montage_Play(equipMontage);
		animInstance->Montage_JumpToSection(sectionName, equipMontage); //play section based on state
	}
}

void Ahscharacter::AttackEndNotify() //making this in c++ instead of bp just for convention sake and practice
{
	actionState = EActionState::EAS_Unoccupied;
}

void Ahscharacter::UnequipNotify()
{

	if (equippedWeapon)
	{
		equippedWeapon->AttachMeshToSocket(GetMesh(), FName("SpineSocket")); //this will attach weapon to new spine socket
	}
}

void Ahscharacter::EquipNotify()
{
	if (equippedWeapon)
	{
		equippedWeapon->AttachMeshToSocket(GetMesh(), FName("RightHandSocketOneHanded")); //this will attach weapon to new spine socket
	}
}

void Ahscharacter::FinishEquipOrUnequip()
{
	actionState = EActionState::EAS_Unoccupied; //setting state back to unoccupied after finishing equip or unequip animation
}

void Ahscharacter::setBoxCollision(ECollisionEnabled::Type collisionEnabled)
{
	if (equippedWeapon && equippedWeapon->GetWeaponBox()) //nullptr check, since it reads left to right, if equippedweapon is null, it will not even check for the boxComp
	{
		equippedWeapon->GetWeaponBox()->SetCollisionEnabled(collisionEnabled); //this will be set in the anim BP, based on if we want to enable or disable collision
		equippedWeapon->actorsToIgnoreTracker.Empty(); //this clears the actors to ignore when we finish our attack so we can attack properly again with the next attack
	}

}

// Called every frame
void Ahscharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void Ahscharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* eInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent); //this casts old comp to new system, cast checked not only casts, but asserts true if all good, if not game crashes

	if (eInput)
	{
		eInput->BindAction(moveForward, ETriggerEvent::Triggered, this, &Ahscharacter::MoveForward);
		eInput->BindAction(look, ETriggerEvent::Triggered, this, &Ahscharacter::Look);
		eInput->BindAction(jumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		eInput->BindAction(equipItemWithE, ETriggerEvent::Started, this, &Ahscharacter::equipItem);
		eInput->BindAction(attack, ETriggerEvent::Triggered, this, &Ahscharacter::Attack);
	}

}
