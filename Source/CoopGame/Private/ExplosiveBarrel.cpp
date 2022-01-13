// Grzanekk made this :)


#include "ExplosiveBarrel.h"
#include "Components/HealthComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AExplosiveBarrel::AExplosiveBarrel()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetMaterial(0, DefaultMaterial);
	RootComponent = MeshComp;

	HealthComp = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	bLitOnFire = false;
}

// Called when the game starts or when spawned
void AExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();

	if(HealthComp)
	{
		HealthComp->OnHealthChanged.AddDynamic(this, &AExplosiveBarrel::OnHealthChanged);
	}
	

}

void AExplosiveBarrel::OnHealthChanged(UHealthComponent* HealthComponent, float Health, float HealthDelta,
	const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if(Health <= 0)
	{
		Explode();
	}
	if(Health <= 50 && !bLitOnFire)
	{
		LightOnFire();
	}
}

void AExplosiveBarrel::LightOnFire()
{
	bLitOnFire = true;

	// UGameplayStatics::SpawnEmitterAttached(FireParticles, MeshComp, );
}

void AExplosiveBarrel::Explode()
{
	MeshComp->SetMaterial(0, ExplodedMaterial);

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionParticles, GetActorLocation());
}

void AExplosiveBarrel::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(bLitOnFire)
	{
		HealthComp->Health -= 2 * DeltaSeconds;
	}
}
