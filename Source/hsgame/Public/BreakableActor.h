// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HitInterface.h"
#include "BreakableActor.generated.h"

class UGeometryCollectionComponent;
class UCapsuleComponent;

UCLASS()
class HSGAME_API ABreakableActor : public AActor, public IHitInterface
{
	GENERATED_BODY()

public:

	ABreakableActor();

	virtual void Tick(float DeltaTime) override;

	virtual void GetHit_Implementation(const FVector& impactPoint) override;

protected:

	virtual void BeginPlay() override;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Geometry")
	UGeometryCollectionComponent* geoCollection;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UCapsuleComponent* capsule;

private:

	UPROPERTY(EditAnywhere, Category = "Properties")
	TArray<TSubclassOf<class ATreasure>> treasureClasses; //getting a ref to the bp treasure class so we can use spawnActor on the bp class, not the cpp class as bp class has mesh info and more
	
	
	//using tsubclassof so that only treasure and its derived classes can be used (in bp, it will now only show class options related to ATreasure)
	//Using array to allow for random treasures

	bool bBroken; //keeping track of broken items to make sure we don't get infinite loop crash

};
