// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeapon.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASWeapon::ASWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkelMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp"));
	RootComponent = SkelMeshComp;

	MuzzleSocketName = "MuzzleSocket";

	Range = 10000.f;
}

// Called when the game starts or when spawned
void ASWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASWeapon::Fire()
{
	AActor* OwnerActor = GetOwner();
	FVector TraceStart;
	FRotator EyeRotation;
	OwnerActor->GetActorEyesViewPoint(TraceStart, EyeRotation);

	FVector ShotDirection = EyeRotation.Vector();
	FVector TraceEnd = TraceStart + ShotDirection * Range;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(OwnerActor);	// Ignoring owner collisions
	QueryParams.AddIgnoredActor(this);	// Ignoring weapon collisions
	QueryParams.bTraceComplex = true;	// Checks for every triangle while hitting target. This helps witch head shot damage
	
	FHitResult Hit;
	if(GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
	{
		UE_LOG(LogTemp, Warning, TEXT("We hit something!"));
		AActor* HitActor = Hit.GetActor();

		UGameplayStatics::ApplyPointDamage(HitActor, 20.f, ShotDirection, Hit, OwnerActor->GetInstigatorController(), this, DamageType);

		if(ImpctEffect)
		{
			UE_LOG(LogTemp, Warning, TEXT("Playing impact effect"));
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpctEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ImpactEffect is nullptr"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("We didn't hit something!"));
	}
	
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Cyan, false, 1.f, 0, 1.f);

	if(MuzzleFlashEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleFlashEffect, SkelMeshComp, MuzzleSocketName);
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

