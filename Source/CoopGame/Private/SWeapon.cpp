// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeapon.h"

#include "DrawDebugHelpers.h"
#include "CoopGame/CoopGame.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

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
	FireRate = .3f;
	
	MaxAmmo = 100;
	MagazineCapacity = 20;

	ReloadTime = 1.2f;

	HeadShotDamageMultiplier = 2.f;
}

void ASWeapon::BeginPlay()
{
	Super::BeginPlay();

	AvailableAmmo = MaxAmmo / 2;
	LoadedAmmo = MagazineCapacity;
}

void ASWeapon::BeginReload()
{
	UE_LOG(LogTemp, Warning, TEXT("Reloading!"));

	GetWorld()->GetTimerManager().SetTimer(TH_ReloadTimer, this, &ASWeapon::Reload, ReloadTime);
}

void ASWeapon::Reload()
{
	int AmmoToReload = MagazineCapacity - LoadedAmmo;
	AmmoToReload = FMath::Min(AmmoToReload, AvailableAmmo);
	
	LoadedAmmo += AmmoToReload;
	AvailableAmmo -= AmmoToReload;

	UE_LOG(LogTemp, Warning, TEXT("Reloaded!"));
}

void ASWeapon::StartFire()
{
	if(LoadedAmmo <= 0)
		return;
	
	float FirstDelay = FMath::Max(LastFireTime + FireRate - GetWorld()->GetTimeSeconds(), 0.f);
	
	GetWorld()->GetTimerManager().SetTimer(TH_FireRate, this, &ASWeapon::Fire, FireRate, true, FirstDelay);	// Sets up loop with delay 'FireRate' seconds between iterations
}

void ASWeapon::StopFire()
{
	GetWorld()->GetTimerManager().ClearTimer(TH_FireRate);
}

void ASWeapon::Fire()
{
	if(LoadedAmmo <= 0)
		return;
	
	AActor* AOwner = GetOwner();

	FVector TraceStart;
	FRotator EyeRotation;
	AOwner->GetActorEyesViewPoint(TraceStart, EyeRotation);

	FVector ShotDirection = EyeRotation.Vector();

	FVector TraceEnd = TraceStart + ShotDirection * Range;

	// FVector MuzzleLocation = SkelMeshComp->GetSocketLocation(MuzzleSocketName);
	// FVector MuzzleRotation = SkelMeshComp->GetSocketRotation(MuzzleSocketName).Vector();

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(AOwner);	// Ignoring owner collisions
	QueryParams.AddIgnoredActor(this);	// Ignoring weapon collisions
	QueryParams.bTraceComplex = true;	// Checks for every triangle while hitting target. This helps witch head shot damage
	QueryParams.bReturnPhysicalMaterial = true;

	FVector TraceEndPoint = TraceEnd;
	
	FHitResult Hit;
	if(GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, COLLISION_WEAPON, QueryParams))
	{
		AActor* HitActor = Hit.GetActor();
		
		EPhysicalSurface SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

		float ActualDamage = Damage;
		if(SurfaceType == SurfaceType2)	// IDK why macro is not working TODO
			ActualDamage *= HeadShotDamageMultiplier;

		UGameplayStatics::ApplyPointDamage(HitActor, ActualDamage, ShotDirection, Hit, AOwner->GetInstigatorController(), this, DamageType);
		
		UParticleSystem* SelectedEffect;
		switch (SurfaceType)
		{
		case SurfaceType1:	// SURFACE_BODYDEFAULT, Macros are not working for some reason :(	TODO
		case SurfaceType2:	// SURFACE_BODYVULNERABLE
			SelectedEffect = BodyImpactEffect;
			break;
		default:
			SelectedEffect = DefaultImpactEffect;
			break;
		}
		if(SelectedEffect)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SelectedEffect, Hit.ImpactPoint, Hit.ImpactNormal.Rotation());
		}

		TraceEndPoint = Hit.ImpactPoint;
	}
	
	PlayFireEffects(TraceStart, TraceEndPoint);

	LastFireTime = GetWorld()->GetTimeSeconds();

	LoadedAmmo--;
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

	APawn* AOwner = Cast<APawn>(GetOwner());
	if(AOwner)
	{
		APlayerController* PC = Cast<APlayerController>(AOwner->GetController());
		if(PC)
		{
			PC->ClientStartCameraShake(FireCameraShake);
		}
	}
}
