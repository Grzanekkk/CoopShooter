// Fill out your copyright notice in the Description page of Project Settings.


#include "GrenadeProjectile.h"

#include "Components/SphereComponent.h"
#include "Components/MeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles//ParticleSystem.h"

AGrenadeProjectile::AGrenadeProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AGrenadeProjectile::OnHit);	// set up a notification for when this component hits something blocking
	CollisionComp->CanCharacterStepUpOn = ECB_No;
	
	RootComponent = CollisionComp;

	MeshComp = CreateDefaultSubobject<UMeshComponent>(TEXT("MeshComp"));
	//MeshComp->SetupAttachment(CollisionComp);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 2000.f;
	ProjectileMovement->MaxSpeed = 2000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	
	InitialLifeSpan = 1.0f;

	ExplosionDamage = DefaultDamageVaule;
	ExplosionRadius = 50.f;
	ExplosionDelay = 1.f;
	DamageType = UDamageType::StaticClass();
}

void AGrenadeProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}

void AGrenadeProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TH_ExplosionDelay, this,  &AGrenadeProjectile::Explode, ExplosionDelay);
}

void AGrenadeProjectile::Explode()
{
	TArray<AActor*> IgnoredActors;
	
	UGameplayStatics::ApplyRadialDamage(GetWorld(), ExplosionDamage, GetActorLocation(), ExplosionRadius, DamageType, IgnoredActors);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticles, GetActorLocation());

	Destroy();
}

void AGrenadeProjectile::SetDamage(float DamageVaule)
{
	if(DamageVaule > 0 && DamageVaule < 100000)
	{
		ExplosionDamage = DamageVaule;
	}
}

inline void AGrenadeProjectile::SetExplosionParticles(UParticleSystem* _ExplosionPartilces)
{
	ExplosionParticles = _ExplosionPartilces;
}
