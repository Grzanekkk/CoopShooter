// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeapon.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

static int32 DebugWeaponDrawing = 0;
FAutoConsoleVariableRef CVARDebugWeaponDrawing (TEXT("COOP.DebugWeapons"), DebugWeaponDrawing, TEXT("Draw Debug Lines for Weapons"), ECVF_Cheat);

// Sets default values
ASWeapon::ASWeapon()
{
	SkelMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp"));
	RootComponent = SkelMeshComp;

	MuzzleSocketName = "MuzzleSocket";
	TraceTargetName = "BeamEnd";

	Range = 10000.f;
	Damage = 20.f;
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

		UGameplayStatics::ApplyPointDamage(HitActor, Damage, ShotDirection, Hit, AOwner->GetInstigatorController(), this, DamageType);

		if(ImpctEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpctEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
		}

		TraceEndPoint = Hit.ImpactPoint;
	}
	
	PlayFireEffects(TraceStart, TraceEndPoint);
}

void ASWeapon::PlayFireEffects(FVector TraceStart, FVector TraceEndPoint) const 
{
	if(DebugWeaponDrawing != 0)
	{
		DrawDebugLine(GetWorld(), TraceStart, TraceEndPoint, FColor::Cyan, false, 1.f, 0, 1.f);
	}

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
