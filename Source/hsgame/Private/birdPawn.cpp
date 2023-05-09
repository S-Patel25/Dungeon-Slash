// Fill out your copyright notice in the Description page of Project Settings.


#include "birdPawn.h"
#include "Components/CapsuleComponent.h" //when only needing certain things, make a habit of putting header files in the CPP file as it gets rid of long complilation times, and circular dependencies
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values
AbirdPawn::AbirdPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule_Component"));
	SetRootComponent(capsule);

	sMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Bird"));
	sMesh->SetupAttachment(GetRootComponent()); //this attaches component to root (same thing as pressing add component on bp screen)

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm")); //attaching components again
	springArm->SetupAttachment(GetRootComponent()); //same thing as saying capsule since its already root component
	springArm->TargetArmLength = 300.f; //setting its arm length

	camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	camera->SetupAttachment(springArm); //setting its parent to springarm
	
	bUseControllerRotationYaw = true;  //set these to true so we can use the controllers yaw and pitch to rotate 
	bUseControllerRotationPitch = true;


}

// Called when the game starts or when spawned
void AbirdPawn::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* controller = Cast<APlayerController>(GetController()); //equivalent of get controller, and cast to player controller in blueprints
	
	if (controller) //null check
	{
		//neat trick if want to use is putting var declaration inside if statement scope so thats its only valid in if statment, can't be used outside
		UEnhancedInputLocalPlayerSubsystem* subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(controller->GetLocalPlayer()); //equivalent of getting subsystem directly in blueprints, getlocalplayer is done automatically in bp, in cpp we do it manually
	
		if (subSystem)
		{
			subSystem->AddMappingContext(birdContext, 0); //final step of adding it to the pawn, NOW all intializing has been done
		}
	}

}

void AbirdPawn::Move(const FInputActionValue& value)
{
	const float cValue = value.Get<float>(); //get float value from IA (make sure its specified to the right value (bool, float, vector, etc.))

	if (Controller && (cValue != 0.f)) //this checks to see if the controller is being possesed and value has to be 1 or -1 (forward or back), then will take forward vector of pawn and use movement input component to add movement
	{
		FVector forward = GetActorForwardVector();
		AddMovementInput(forward, cValue);
	}
}

void AbirdPawn::Look(const FInputActionValue& value)
{
	const FVector2D vectorValue = value.Get<FVector2d>();

	if (Controller) //checks too see if controller is valid
	{
		AddControllerYawInput(vectorValue.X); //this uses the add input controller method to move based on mouse rotation
		AddControllerPitchInput(vectorValue.Y);
	}


}

// Called every frame
void AbirdPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AbirdPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* eInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent); //this casts old comp to new system, cast checked not only casts, but asserts true if all good, if not game crashes

	if (eInput)
	{
		eInput->BindAction(input, ETriggerEvent::Triggered, this, &AbirdPawn::Move); //how to bind IA, first param is IA, second is what state (triggered, pressed etc.), third is object (usually self since we want to move object in code), last is FULL address of method being used (thats what & is for)
		eInput->BindAction(look, ETriggerEvent::Triggered, this, &AbirdPawn::Look);
	}

}

