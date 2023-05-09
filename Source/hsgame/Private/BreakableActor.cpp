// Fill out your copyright notice in the Description page of Project Settings.


#include "BreakableActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Components/CapsuleComponent.h"
#include "Treasure.h"

ABreakableActor::ABreakableActor()
{
	PrimaryActorTick.bCanEverTick = false;

	geoCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("Geometry Component"));
	SetRootComponent(geoCollection);

	capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	capsule->SetupAttachment(GetRootComponent());

	geoCollection->SetGenerateOverlapEvents(true);
	geoCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	geoCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);

}

void ABreakableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABreakableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABreakableActor::GetHit_Implementation(const FVector& impactPoint)
{
	if (bBroken) return;

	bBroken = true;

	if (GetWorld() && treasureClasses.Num() > 0) //nullptr check
	{
		FVector location = GetActorLocation();
		location.Z += 45.f; //adding 75 units to put it in air rather then ground

		int32 randTreasureIndex = FMath::RandRange(0, treasureClasses.Num() - 1); //getting a random index

		GetWorld()->SpawnActor<ATreasure>(treasureClasses[randTreasureIndex], location, GetActorRotation()); //spawnactor is a template function, so make sure to specify type
	}

}

