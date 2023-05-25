// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimMontage.h"
#include "Kismet/GameplayStatics.h"
#include "AttributeComponent.h"
#include "HealthBarComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "Perception/PawnSensingComponent.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block); //setting collision properties in constructor
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

	attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

	healthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("Health Bar Widget"));
	healthBarWidget->SetupAttachment(GetRootComponent()); //as this will be on top of enemy head, we need it attached

	pawnSense = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sense Component"));
	pawnSense->SetPeripheralVisionAngle(45.f);
	pawnSense->SightRadius = 4000.f; //setting values in cpp for practice, can also be done in blueprints (better in bp so you can visually see)

	GetCharacterMovement()->bOrientRotationToMovement = true; //setting these settings in both BP's and cpp
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	if(healthBarWidget)
	{
		healthBarWidget->SetVisibility(false); //setting health bar to hidden until enemy gets hit
	}
	
	enemyController = Cast<AAIController>(GetController()); //casting from AController to ai controlller
	moveToTarget(patrolTarget);

	if (pawnSense)
	{
		pawnSense->OnSeePawn.AddDynamic(this, &AEnemy::pawnSighted); //using onseepawn delete function and binding that custom function so it can be used along with other functionality (changing state, speed, etc.)
	}
}

void AEnemy::Death()
{
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance(); 

	if (animInstance && deathMontage) 
	{
		animInstance->Montage_Play(deathMontage);  

		int32 randAttack = FMath::RandRange(0, 4); //play random death animation
		FName sectionName = FName();

		switch (randAttack)
		{
		case 0:
			sectionName = FName("Death1"); 
			deathPose = EDeathPose::EDP_Death1;
			break;
		case 1:
			sectionName = FName("Death2");
			deathPose = EDeathPose::EDP_Death2;
			break;
		case 2:
			sectionName = FName("Death3");
			deathPose = EDeathPose::EDP_Death3;
			break;
		case 3:
			sectionName = FName("Death4");
			deathPose = EDeathPose::EDP_Death4;
			break;
		case 4:
			sectionName = FName("Death5");
			deathPose = EDeathPose::EDP_Death5;
			break;
		}

		animInstance->Montage_JumpToSection(sectionName, deathMontage);
	}

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetLifeSpan(5.f); //disable enemy capsule component so we can walk through when its dead, and then destroy it after 3 seconds

	if (healthBarWidget)
	{
		healthBarWidget->SetVisibility(false); 
	}
}

bool AEnemy::inTargetRange(AActor* target, double radius) //makes calculating range easier
{
	if (target == nullptr) return false;
	const double distanceToEnemy = (target->GetActorLocation() - GetActorLocation()).Size();
	return distanceToEnemy <= radius;
}

void AEnemy::moveToTarget(AActor* target)
{
	if (enemyController == nullptr || target == nullptr) return;

	FAIMoveRequest moveRequest;
	moveRequest.SetGoalActor(target); //uses this structs functions to set patrol target as point to where enemy will move to
	moveRequest.SetAcceptanceRadius(10.f);

	FNavPathSharedPtr navPath;

	enemyController->MoveTo(moveRequest);

}

AActor* AEnemy::choosePatrolTarget() //refactored method from tick function, now will return target actor which can be used for other functionality
{
	TArray<AActor*> validTargets;

	for (AActor* target : patrolTargets)
	{
		if (target != patrolTarget)
		{
			validTargets.AddUnique(target); //will make sure each target is valid (not the same one each time)
		}
	}

	const int32 numTargets = validTargets.Num();

	if (numTargets > 0)
	{
		const int32 randTargetSelection = FMath::RandRange(0, numTargets - 1); //making a random range of patrol targets
		return validTargets[randTargetSelection];
	}
		
	return nullptr;
}

void AEnemy::pawnSighted(APawn* seenPawn)
{
	if (enemyState == EEnemyState::EES_Chasing) return; //simple check to see if enemy is already chasing then return so it does not call function multiple times

	if (seenPawn->ActorHasTag(FName("Character"))) //checks to see if enemy is looking at player
	{
		GetWorldTimerManager().ClearTimer(patrolTimer); //clear the timer so enemy does not go back to patrolling randomly
		GetCharacterMovement()->MaxWalkSpeed = 300.f; //increase speed so enemy is faster
		combatTarget = seenPawn;

		if (enemyState != EEnemyState::EES_Attacking)
		{
			enemyState = EEnemyState::EES_Chasing;
			moveToTarget(combatTarget);
		}
		
	}





}

void AEnemy::playHitReactMontage(const FName& sectionName)
{
	UAnimInstance* animInstance = GetMesh()->GetAnimInstance();

	if (animInstance && hitReactMontage)
	{
		animInstance->Montage_Play(hitReactMontage);
		animInstance->Montage_JumpToSection(sectionName, hitReactMontage); //play section based on state
	}
}

void AEnemy::patrolTimerFinished()
{
	moveToTarget(patrolTarget);
}


void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//const double distanceToEnemy = (combatTarget->GetActorLocation() - GetActorLocation()).Size(); //this gets the distance between enemy and player, so we can hide or show health bar based on distance

	if (enemyState > EEnemyState::EES_Patrolling) //since enums are ints, this is basically saying if the enemystate is either attacking or chasing, check for combat target
	{
		checkCombatTarget();
	}
	else //if no combat target, then just patrol normally
	{
		checkPatrolTarget();
	}

}

void AEnemy::checkPatrolTarget()
{
	if (inTargetRange(patrolTarget, patrolRadius))
	{
		patrolTarget = choosePatrolTarget(); //using refactored functions to clean up code
		GetWorldTimerManager().SetTimer(patrolTimer, this, &AEnemy::patrolTimerFinished, 5.f); //using getworldtimer to create "idle" patrolling movement for a time, then enemy will start moving again
	}
}

void AEnemy::checkCombatTarget()
{
	if (!inTargetRange(combatTarget, combatRadius)) 
	{
		combatTarget = nullptr; //outside combat radius, so enemy will lose interest
		if (healthBarWidget)
		{
			healthBarWidget->SetVisibility(false); //disable health bar from showing when we are far away from enemy
		}
		enemyState = EEnemyState::EES_Patrolling;
		GetCharacterMovement()->MaxWalkSpeed = 100.f;
		moveToTarget(patrolTarget); //since there is still a patrol target (not player anymore), enemy will start moving there
	}
	else if(!inTargetRange(combatTarget, attackRadius) && enemyState != EEnemyState::EES_Chasing)
	{
		//chase player
		enemyState = EEnemyState::EES_Chasing;
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		moveToTarget(combatTarget);
	}
	else if (inTargetRange(combatTarget, attackRadius) && enemyState != EEnemyState::EES_Attacking)
	{
		//attack player
		enemyState = EEnemyState::EES_Attacking;
	}
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::GetHit_Implementation(const FVector& impactPoint)
{
	if (healthBarWidget)
	{
		healthBarWidget->SetVisibility(true);
	}


	if (attributes && attributes->isAlive())
	{
		DirectionalHitReact(impactPoint); //only hit if character is alive
	}
	else
	{
		Death();
	}

	if (hitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, hitSound, impactPoint); //will play the hit sound when enemy is hit
	}
	if (hitParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, hitParticles, impactPoint); //will spawn hit particles at impact point
	}
}

void AEnemy::DirectionalHitReact(const FVector& impactPoint)
{
	const FVector forward = GetActorForwardVector();

	//lower the impact point to actors Z location so it is paralell
	const FVector loweredImpact(impactPoint.X, impactPoint.Y, GetActorLocation().Z);

	const FVector toHit = (loweredImpact - GetActorLocation()).GetSafeNormal(); //this vector is the vector from where the enemy got hit, to the location of the actor (make it normalized so dot product is easier to do)

	//forward * toHit = |forward||toHit| * cost(theta)
	//forward = 1, toHit = 1, since normalized vector, so forward * toHit = cos(theta)
	const double cosTheta = FVector::DotProduct(forward, toHit); //performing dot product on these two vectors so we can get the angle between forward and hit, so we can use that angle to determine which way enemy will stumble (directional hit)

	//using inverse to get angle (theta)
	double theta = FMath::Acos(cosTheta);

	//now we convert from radians to degrees (final step)
	theta = FMath::RadiansToDegrees(theta);

	//using cross product so we can see the negative angles too for directional hit (as dot product returns scalar, and cross product returns vector pointing up or down, up is +ve, down is -ve)
	const FVector crossProduct = FVector::CrossProduct(forward, toHit);

	if (crossProduct.Z < 0)
	{
		theta *= -1.f; //making angle negative if its pointing downwards
	}

	FName sectionName("FromBack"); //using any section here, so we only check for the other three

	if (theta >= -45.f && theta < 45.f)
	{
		sectionName = FName("FromFront");
	}
	else if (theta >= -135.f && theta < -45.f)
	{
		sectionName = FName("FromLeft");
	}
	else if (theta >= 45.f && theta < 135.f)
	{
		sectionName = FName("FromRight");
	}

	playHitReactMontage(sectionName); //now after all the math is done, we play the montage based on the angle of theta
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (attributes && healthBarWidget)
	{
		attributes->tookDamage(DamageAmount);
		healthBarWidget->SetHealthPercent(attributes->getHealthPercent()); //setting health for the widget based on damage
	}

	combatTarget = EventInstigator->GetPawn();
	enemyState = EEnemyState::EES_Chasing; //makes it so enemy will agro when hit behind or anywhere where its not looking
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	moveToTarget(combatTarget);
	return DamageAmount;
}

