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
class AAIController;
class UPawnSensingComponent;

UCLASS()
class HSGAME_API AEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	AEnemy();

	virtual void Tick(float DeltaTime) override;
	void checkPatrolTarget();
	void checkCombatTarget();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetHit_Implementation(const FVector& impactPoint) override; //because its native event, we mark it as implementation

	void DirectionalHitReact(const FVector& impactPoint);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override; //using ue5 built in damage system



protected:
	virtual void BeginPlay() override;
	
	void Death();

	bool inTargetRange(AActor* target, double radius);

	void moveToTarget(AActor* target);
	AActor* choosePatrolTarget();

	UFUNCTION() //remember to add UFUNCTION when dealing with delegates and callback functions
	void pawnSighted(APawn* seenPawn);

	UPROPERTY(BlueprintReadOnly)
	EDeathPose deathPose = EDeathPose::EDP_Alive;


	//mongtages

	void playHitReactMontage(const FName& sectionName);



private:

	//components

	UPROPERTY(VisibleAnywhere)
	UAttributeComponent* attributes;

	UPROPERTY(VisibleAnywhere)
	UHealthBarComponent* healthBarWidget;

	UPROPERTY(VisibleAnywhere)
	UPawnSensingComponent* pawnSense;

	//montages

	UPROPERTY(EditDefaultsOnly, Category = "Montages") //make sure its exposed to ue's reflection system
	UAnimMontage* hitReactMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Montages") //make sure its exposed to ue's reflection system
	UAnimMontage* deathMontage;

	//sounds and vfx

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* hitSound;

	UPROPERTY(EditAnywhere, Category = "VFX")
	UParticleSystem* hitParticles;

	//nav and ai

	UPROPERTY() //good rule of thumb to add UPROPERTY to actor pointers
	AAIController* enemyController;

	UPROPERTY(EditInstanceOnly, Category = "AINav")
	AActor* patrolTarget; //refers to current patrol target

	UPROPERTY(EditInstanceOnly, Category = "AINav")
	TArray<AActor*> patrolTargets; //array so we can store multiple patrol points

	UPROPERTY(EditAnywhere)
	double patrolRadius = 200.f;

	UPROPERTY()
	AActor* combatTarget;

	UPROPERTY(EditAnywhere)
	double combatRadius = 500.f;

	UPROPERTY(EditAnywhere)
	double attackRadius = 150.f;

	FTimerHandle patrolTimer;
	void patrolTimerFinished();

	//enum

	EEnemyState enemyState = EEnemyState::EES_Patrolling;

public:	


};
