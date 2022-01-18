// Fill out your copyright notice in the Description page of Project Settings.


#include "GrenadeProjectile.h"

#include "DrawDebugHelpers.h"
#include "ExplosiveBarrel.h"
#include "Components/SphereComponent.h"
#include "Components/MeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "SCharacter.h"


AGrenadeProjectile::AGrenadeProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->SetCollisionProfileName("Projectile");
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	CollisionComp->SetSimulatePhysics(true);
	RootComponent = CollisionComp;

	MeshComp = CreateDefaultSubobject<UMeshComponent>(TEXT("MeshComp"));
	//MeshComp->SetupAttachment(RootComponent);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 2000.f;
	ProjectileMovement->MaxSpeed = 2000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	
	//InitialLifeSpan = 1.0f;

	// Setting default values
	ExplosionDamage = DefaultDamageValue;
	ExplosionRadius = 50.f;
	ExplosionDelay = 1.f;
	DamageType = UDamageType::StaticClass();
}

void AGrenadeProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionComp->OnComponentHit.AddDynamic(this, &AGrenadeProjectile::OnHit);	// set up a notification for when this component hits something blocking
}

void AGrenadeProjectile::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	ExplosionDelay -= DeltaSeconds;

	if(ExplosionDelay <= 0)
	{
		Explode();
	}
}

void AGrenadeProjectile::Explode()
{
	TArray<AActor*> IgnoreActors;	// TODO
	
	UGameplayStatics::ApplyRadialDamage(GetWorld(), ExplosionDamage, GetActorLocation(), ExplosionRadius, DamageType, IgnoreActors);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticles, GetActorLocation());
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplosionCue, GetActorLocation());

	DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius, 8, FColor::Orange, false, 3.f);
	
	Destroy();
}

void AGrenadeProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Projectile Colliding"));
	
	if(OtherActor->IsA(ASCharacter::StaticClass()) || OtherActor->IsA(AExplosiveBarrel::StaticClass()))
	{
		Explode();
	}
}

void AGrenadeProjectile::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Projectile Overlapping"));
	Explode();
}