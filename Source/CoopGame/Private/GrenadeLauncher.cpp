// Fill out your copyright notice in the Description page of Project Settings.


#include "GrenadeLauncher.h"
#include "GrenadeProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

AGrenadeLauncher::AGrenadeLauncher()
{

	SkelMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	RootComponent = SkelMeshComp;

	MuzzleSocketName = "MuzzleSocket";

	Damage = 50.f;
	ExplosionRadius = 100.f;
	ExplosionDelay = 1.f;

	SetReplicates(true);
}

void AGrenadeLauncher::Fire()
{
	UE_LOG(LogTemp, Warning, TEXT("Shooting!!"));
	AActor* AOwner = GetOwner();
	
	if(ProjectileClass && AOwner)
	{
		FVector TraceStart;
		FRotator LaunchRotation;
		AOwner->GetActorEyesViewPoint(TraceStart, LaunchRotation);

		LaunchRotation += FRotator(10.f, 0.f, 0.f);
		
		FVector MuzzleLocation = SkelMeshComp->GetSocketLocation(MuzzleSocketName);
		FRotator MuzzleRotation = SkelMeshComp->GetSocketRotation(MuzzleSocketName);
		
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		ActorSpawnParams.Instigator = GetOwner()->GetInstigatorController()->GetPawn();
		
		AGrenadeProjectile* Projectile = GetWorld()->SpawnActor<AGrenadeProjectile>(ProjectileClass, MuzzleLocation, LaunchRotation, ActorSpawnParams);
		if(Projectile)	
		{
			UE_LOG(LogTemp, Warning, TEXT("Changing projectile parameters"));
			
			Projectile->SetDamage(50);
			Projectile->SetExplosionParticles(ExplosionEffect);
			Projectile->SetExplosionRadius(ExplosionRadius);
			Projectile->SetExplosionDelay(ExplosionDelay);
		}

		UGameplayStatics::PlaySoundAtLocation(GetWorld(), LaunchCue, GetActorLocation(), FRotator::ZeroRotator);
	}
}