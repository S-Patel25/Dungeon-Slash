// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponClass.h"
#include "hscharacter.h" //dont forget to include header files when using other components from other files
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "HitInterface.h"

AWeaponClass::AWeaponClass() //constructor
{
	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	boxComp->SetupAttachment(GetRootComponent()); //setting up box component for weapon
	boxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision); //setting col responses in c++
	boxComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	boxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	start = CreateDefaultSubobject<USceneComponent>(TEXT("Start")); //setting up start and end scence components for box trace
	start->SetupAttachment(GetRootComponent());

	end = CreateDefaultSubobject<USceneComponent>(TEXT("End"));
	end->SetupAttachment(GetRootComponent());
}

void AWeaponClass::BeginPlay()
{
	Super::BeginPlay();

	boxComp->OnComponentBeginOverlap.AddDynamic(this, &AWeaponClass::OnBoxOverlap);	

}

void AWeaponClass::Equip(USceneComponent* InParent, FName InSocketName)
{
	AttachMeshToSocket(InParent, InSocketName); //now that we've made a method for attach to component, we no longer need to cast
	itemState = EItemState::EIS_Equipped;
	if (equipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, equipSound, GetActorLocation()); //playing equip sound at weapon location
	}

	if (sphereComp)
	{
		sphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision); //setting the weapon to have to collision once equipped
	}
	
}

void AWeaponClass::AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true); //using the enum rules from attach to component (we want snap to target for all 3)
	mesh->AttachToComponent(InParent, TransformRules, InSocketName);
}

void AWeaponClass::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult); //calling super (parent class method) so we get access to everything from parent class
}

void AWeaponClass::OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);


}

void AWeaponClass::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FVector startLoc = start->GetComponentLocation();
	const FVector endLoc = end->GetComponentLocation(); //getting location of start and end of sword to perform box trace

	FHitResult boxHit; //making a hitresult var to see what information we get when weapon hits something (as the method takes the hit results as a OUT)

	TArray<AActor*> actorsToIgnore;

	actorsToIgnore.Add(this); //adding weapon to ignore actors

	for (AActor* actor : actorsToIgnoreTracker)
	{
		actorsToIgnore.AddUnique(actor);
	}

	UKismetSystemLibrary::BoxTraceSingle(this, startLoc, endLoc, FVector(5.f, 5.f, 5.f), start->GetComponentRotation(), ETraceTypeQuery::TraceTypeQuery1, false, actorsToIgnore, EDrawDebugTrace::None, boxHit, true);

	if (boxHit.GetActor())
	{
		IHitInterface* hitInterface = Cast<IHitInterface>(boxHit.GetActor()); //casting to see if the actor hit implements the interface

		if (hitInterface)
		{
			hitInterface->Execute_GetHit(boxHit.GetActor(), boxHit.ImpactPoint); //using auto-generated method as we made in blueprint native
		}
		actorsToIgnoreTracker.AddUnique(boxHit.GetActor()); //adding actor to ignore array

		CreateFields(boxHit.ImpactPoint); //passing in impact point so we can destroy meshes with sword
	}

}