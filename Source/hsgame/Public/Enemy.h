// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HitInterface.h"
#include "Enemy.generated.h"

class UAnimMontage;

UCLASS()
class HSGAME_API AEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	AEnemy();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetHit_Implementation(const FVector& impactPoint) override; //because its native event, we mark it as implementation

	void DirectionalHitReact(const FVector& impactPoint);

protected:
	virtual void BeginPlay() override;

	void playHitReactMontage(const FName& sectionName);


private:

	//montages

	UPROPERTY(EditDefaultsOnly, Category = "Montages") //make sure its exposed to ue's reflection system
	UAnimMontage* hitReactMontage;


	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* hitSound;

	UPROPERTY(EditAnywhere, Category = "VFX")
	UParticleSystem* hitParticles;



public:	


};
