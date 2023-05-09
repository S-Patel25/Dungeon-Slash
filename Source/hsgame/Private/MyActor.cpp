// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"
#include "hsgame/DebugMacros.h" //use custom header files instead of putting everything in one header file for readability and modularity
#include "Components/SphereComponent.h"
#include "hscharacter.h"


//WHEN LARGE BLOCKS ARE CODE ARE COMMENTED OUT, its simply because of new content being added

// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("meshComponent")); //after making pointer, we intialize with subObject so it can be seen and edited
	SetRootComponent(mesh);

	sphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	sphereComp->SetupAttachment(GetRootComponent());

}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();

	sphereComp->OnComponentBeginOverlap.AddDynamic(this, &AMyActor::OnSphereOverlap); //this line of code here binds the delegate to the callback function to allow it to be used
	sphereComp->OnComponentEndOverlap.AddDynamic(this, &AMyActor::OnSphereOverlapEnd); //first param is object (so this), second is the address of method we made
	

	//UE_LOG(LogTemp, Warning, TEXT("Hello LOL")); //make sure to always use TEXT() for strings

	//UWorld* world = GetWorld(); //get UWorld

	//SetActorLocation(FVector((180.f, 70.f, 3350.f))); //setting actor location using temp constructor (FVector), also make sure to move location code below this as this gets called BEFORE draw sphere
	//SetActorRotation(FRotator(0.f, 45.f, 35.f)); //setting actor rotation with FRotator (pitch, roll, yaw) all angles
	
	//FVector actorLocation = GetActorLocation(); //get location of bp actor
	//FVector actorForwardLocation = GetActorForwardVector() * 100.f; //forward vector is a normalized vector (unit vector), so multiplyign by 100 to have bigger line
	//adding these two gets forward vector in straight line

	//DRAW_SPHERE(GetActorLocation());
	/*DRAW_LINE(actorLocation, actorLocation + actorForwardLocation);
	DRAW_POINT(actorLocation + actorForwardLocation);*/
	//DRAW_VECTOR(actorLocation, actorLocation + actorForwardLocation);
	//DRAW_BOX(actorLocation, FColor::Cyan);

}

float AMyActor::transSinFunction()
{
	return amplitude * FMath::Sin(runTime * waveSpeed);
}

void AMyActor::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	Ahscharacter* character = Cast<Ahscharacter>(OtherActor); //get a reference of the character

	if (character)
	{
		character->SetOverlappingItem(this); //setting the overlapping item to "this" (the item object)
	}



}

void AMyActor::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Ahscharacter* character = Cast<Ahscharacter>(OtherActor); //get a reference of the character

	if (character)
	{
		character->SetOverlappingItem(nullptr); //setting it to null once the player is out of range
	}



}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); //deltatime is used for constant check as not all systems have the same framerate


	////movement rate is in cm/s
	//float movementRate = 50.f;
	float rotationRate = 50.f;

	////movement rate * deltatime = (cm/s) * (s/frame) = cm/frame)
	//AddActorWorldOffset(FVector(movementRate * DeltaTime, 0.f, 0.f)); //always be using var * deltatime as movement is now SCALED by deltatime rather then the systems framerate because its independent to framerate
	//AddActorWorldRotation(FRotator(0.f, rotationRate * DeltaTime, 0.f));

	runTime += DeltaTime;

	float zMovement = amplitude * FMath::Sin(runTime * waveSpeed); //FMath, another library, zMovement as we want object to move up and down, value before is amplitude (how far up and down wave goes), and value after runTime is how fast wave will go (period = 2pi/k, where k is speed)


	if (itemState == EItemState::EIS_Hovering) //make it hover if its not equipped
	{
		AddActorWorldOffset(FVector(0.f, 0.f, zMovement));
		AddActorWorldRotation(FRotator(0.f, rotationRate * DeltaTime, 0.f));
	}
	
	//DRAW_SPHERE_SingleFrame(GetActorLocation());
	//DRAW_VECTOR_SingleFrame(GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 100.f);

	//FVector avgVector = Avg<FVector>(GetActorLocation(), FVector::ZeroVector); //using template type

	//DRAW_POINT_SingleFrame(avgVector);









}

