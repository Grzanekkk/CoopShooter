// Fill out your copyright notice in the Description page of Project Settings.


#include "GrenadeLauncher.h"
#include "GrenadeProjectile.h"

// Sets default values
AGrenadeLauncher::AGrenadeLauncher()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkelMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	RootComponent = SkelMeshComp;

	MuzzleSocketName = "MuzzleSocket";

	Damage = 50.f;
	ExplosionRadius = 50.f;
	ExplosionDelay = 1.f;
}

// Called when the game starts or when spawned
void AGrenadeLauncher::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void AGrenadeLauncher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
		if(Projectile)	// Currently not working	
		{
			// TODO
			UE_LOG(LogTemp, Warning, TEXT("Changing projectile parameters"));
			
			Projectile->SetDamage(50);
			Projectile->SetExplosionParticles(ExplosionEffect);
			Projectile->SetExplosionRadius(ExplosionRadius);
			Projectile->SetExplosionDelay(ExplosionDelay);
		}
	}
}
