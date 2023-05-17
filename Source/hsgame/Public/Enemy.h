// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HitInterface.h"
#include "CharacterTypesEnum.h"
#include "Enemy.generated.h"

class UAnimMontage;
class UAttributeComponent;
class UHealthBarComponent;

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

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override; //using ue5 built in damage system



protected:
	virtual void BeginPlay() override;
	
	void Death();

	UPROPERTY(BlueprintReadOnly)
	EDeathPose deathPose = EDeathPose::EDP_Alive;

	//mongtages

	void playHitReactMontage(const FName& sectionName);



private:

	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* attributes;

	//montages

	UPROPERTY(EditDefaultsOnly, Category = "Montages") //make sure its exposed to ue's reflection system
	UAnimMontage* hitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montages") //make sure its exposed to ue's reflection system
	UAnimMontage* deathMontage;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* hitSound;

	UPROPERTY(EditAnywhere, Category = "VFX")
	UParticleSystem* hitParticles;

	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* healthBarWidget;

	UPROPERTY()
	AActor* combatTarget;

	UPROPERTY(EditAnywhere)
	double combatRadius = 500.f;

public:	


};
