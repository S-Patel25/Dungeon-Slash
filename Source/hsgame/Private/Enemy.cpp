// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimMontage.h"
#include "Kismet/GameplayStatics.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block); //setting collision properties in constructor
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);

}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
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


void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::GetHit_Implementation(const FVector& impactPoint)
{
	DirectionalHitReact(impactPoint);

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

