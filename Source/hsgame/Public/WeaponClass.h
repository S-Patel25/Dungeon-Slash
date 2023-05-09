// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyActor.h"
#include "WeaponClass.generated.h"

class USoundBase;
class UBoxComponent;

/**
 * 
 */
UCLASS()
class HSGAME_API AWeaponClass : public AMyActor
{
	GENERATED_BODY()


public:

	AWeaponClass();

	void Equip(USceneComponent* InParent, FName InSocketName);

	void AttachMeshToSocket(USceneComponent* InParent, const FName& InSocketName);

	FORCEINLINE UBoxComponent* GetWeaponBox() const { return boxComp; }

	TArray<AActor*> actorsToIgnoreTracker; //keeps track of what we've already hit

protected:

	virtual void BeginPlay() override;

	//even modifiers like uproperty and ufunction are inherited, so no need to include them

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override; //this is using delegate method for collision, has to be the same params for it to work
	virtual void OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override; //make sure to mark as override

	UFUNCTION() //make sure its exposed to reflection system so it can be bound to overlap delegate
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION(BlueprintImplementableEvent)
	void CreateFields(const FVector& fieldLocation); //for destrucible meshes

private:

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	USoundBase* equipSound;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	UBoxComponent* boxComp;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	USceneComponent* start;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Properties")
	USceneComponent* end;
};
