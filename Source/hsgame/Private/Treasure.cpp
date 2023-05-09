// Fill out your copyright notice in the Description page of Project Settings.


#include "Treasure.h"
#include "hscharacter.h"
#include "Kismet/GameplayStatics.h"

void ATreasure::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Ahscharacter* character = Cast<Ahscharacter>(OtherActor); //get a reference of the character

	if (character)
	{
		UGameplayStatics::PlaySoundAtLocation(this, pickupSound, GetActorLocation()); //play sound, then destroy actor
		Destroy(); //removes actor
	}
}
