// Grzanekk made this :)


#include "ExplosiveBarrel.h"

#include "DrawDebugHelpers.h"
#include "Components/HealthComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AExplosiveBarrel::AExplosiveBarrel()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetMaterial(0, DefaultMaterial);
	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetCollisionObjectType(ECC_PhysicsBody);
	RootComponent = MeshComp;

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
	RadialForceComp->SetupAttachment(MeshComp);
	RadialForceComp->bImpulseVelChange = true;
	RadialForceComp->bAutoActivate = false;
	RadialForceComp->bIgnoreOwningActor = true;	// Ignore self

	bExploded = false;
	bLitOnFire = false;
	Damage = 100;
	ExplosionRadius = 200;
	BarrelBoostImpulse = 400;
}

// Called when the game starts or when spawned
void AExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();

	RadialForceComp->Radius = 200;
	
	HealthComp->OnHealthChanged.AddDynamic(this, &AExplosiveBarrel::OnHealthChanged);
}

void AExplosiveBarrel::OnHealthChanged(UHealthComponent* HealthComponent, float Health, float HealthDelta,
	const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if(!bExploded)
	{
		if(Health <= 0)
		{
			Explode();
		}
		else if(Health <= 50 && !bLitOnFire)
		{
			LightOnFire();
		}
	}
}

void AExplosiveBarrel::LightOnFire()
{
	bLitOnFire = true;

	UGameplayStatics::SpawnEmitterAttached(FireParticles, MeshComp);
}

void AExplosiveBarrel::Explode()
{
	bExploded = true;
	
	TArray<AActor*> IgnoredActors; 
	
	UGameplayStatics::ApplyRadialDamage(GetWorld(), Damage, GetActorLocation(), ExplosionRadius, ExplosionDamageType, IgnoredActors, this);

	FVector BoostStrength = FVector::UpVector * BarrelBoostImpulse;
	MeshComp->AddImpulse(BoostStrength, NAME_None, true);
	RadialForceComp->FireImpulse();

	DrawDebugSphere(GetWorld(), GetActorLocation(), ExplosionRadius, 12, FColor::Red, false, 5);
	
	MeshComp->SetMaterial(0, ExplodedMaterial);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticles, GetActorLocation());
}

void AExplosiveBarrel::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(bLitOnFire)
	{
		//HealthComp->Health -= 2 * DeltaSeconds;
	}
}
