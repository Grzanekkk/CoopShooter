// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeapon.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASWeapon::ASWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkelMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp"));
	RootComponent = SkelMeshComp;

	MuzzleSocketName = "MuzzleSocket";
	TraceTargetName = "BeamEnd";

	Range = 10000.f;
}

// Called when the game starts or when spawned
void ASWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASWeapon::Fire()
{
	AActor* AOwner = GetOwner();

	FVector TraceStart;
	FRotator EyeRotation;
	AOwner->GetActorEyesViewPoint(TraceStart, EyeRotation);

	FVector ShotDirection = EyeRotation.Vector();

	// FVector MuzzleLocation = SkelMeshComp->GetSocketLocation(MuzzleSocketName);
	// FVector MuzzleRotation = SkelMeshComp->GetSocketRotation(MuzzleSocketName).Vector();
	
	FVector TraceEnd = TraceStart + ShotDirection * Range;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(AOwner);	// Ignoring owner collisions
	QueryParams.AddIgnoredActor(this);	// Ignoring weapon collisions
	QueryParams.bTraceComplex = true;	// Checks for every triangle while hitting target. This helps witch head shot damage

	FVector TraceEndPoint = TraceEnd;
	
	FHitResult Hit;
	if(GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
	{
		AActor* HitActor = Hit.GetActor();

		UGameplayStatics::ApplyPointDamage(HitActor, 20.f, ShotDirection, Hit, AOwner->GetInstigatorController(), this, DamageType);

		if(ImpctEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpctEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
		}

		TraceEndPoint = Hit.ImpactPoint;
	}
	
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Cyan, false, 1.f, 0, 1.f);

	if(MuzzleFlashEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleFlashEffect, SkelMeshComp, MuzzleSocketName);
	}

	if(TraceEffect)
	{
		UParticleSystemComponent* TracerComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TraceEffect, TraceStart);
		if(TracerComp)
		{
			TracerComp->SetVectorParameter(TraceTargetName, TraceEndPoint);
		}
	}
}

void ASWeapon::PlayFiringEffects()
{
		// IDK if i want to use this function :|
}

// Called every frame
void ASWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

