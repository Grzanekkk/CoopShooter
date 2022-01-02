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
	if(ProjectileClass)
	{
		FVector MuzzleLocation = SkelMeshComp->GetSocketLocation(MuzzleSocketName);
		FRotator MuzzleRotation = SkelMeshComp->GetSocketRotation(MuzzleSocketName);

		//Set Spawn Collision Handling Override
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		ActorSpawnParams.Instigator = GetOwner()->GetInstigatorController()->GetPawn();

		// spawn the projectile at the muzzle
		GetWorld()->SpawnActor<AGrenadeProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, ActorSpawnParams);
	}
}
