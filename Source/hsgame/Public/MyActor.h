// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActor.generated.h"

class USphereComponent;

enum class EItemState : uint8 //control status of item so we can apply different behaviour too it
{
	EIS_Hovering,
	EIS_Equipped
};

UCLASS()
class HSGAME_API AMyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sin Functions") //bpreadwrite exposes vars to event graph, BUT make sure that this is used on pub or protected, does not work for private variables
		float amplitude = 1.f;												//category adds specifcity to project, to make sure not everything is in one category
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sin Functions") //UPROPERTY defines properties a var or func can have (editanywhere for example means that a user can edit both an instance AND the default BP values), exposes it to blueprints system
		float waveSpeed = 5.f;

	UFUNCTION(BlueprintPure) //UFunction is for methods, bpPure means no execution pins (just get data and return), bpCallable means it has the two execution pins
		float transSinFunction(); //practicing making functions exposed to bp

	UFUNCTION() //has to be exposed to ue's reflection system so it can be bound
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult); //this is using delegate method for collision, has to be the same params for it to work

	

	//A delegate is an event that can be called and used (think of it like a callable object)
	//making these methods virtual so we can override in inherited classes

	UFUNCTION()
	virtual void OnSphereOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* mesh; //create pointer for static mesh component with visibleAnywhere (so we can see in instance and bp)

	template<typename T> //templates allow for more functionality with specific types (for example, adding two vectors or floats or whatever, so same thing as function, but now we can define a type along with it)
	T Avg(T firstVar, T secondVar);

	EItemState itemState = EItemState::EIS_Hovering; //putting this in protected so inherited classes can use it

	UPROPERTY(VisibleAnywhere)
	USphereComponent* sphereComp;
	
private:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true")) //makes it so you can see, not edit (good for vars that should not be changed or edited, but still good to expose to bp to see effects, use META property to access private variables bp
	float runTime = 0.f;
	
};

template<typename T>
inline T AMyActor::Avg(T firstVar, T secondVar)
{
	return T((firstVar + secondVar) / 2); //geting avg
}
