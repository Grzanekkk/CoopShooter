// Grzanekk made this :)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExplosiveBarrel.generated.h"

UCLASS()
class COOPGAME_API AExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	AExplosiveBarrel();

	UPROPERTY(EditDefaultsOnly, Category = "Properties")
	float Damage;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UHealthComponent* HealthComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Materaials")
	class UMaterial* DefaultMaterial;
	
	UPROPERTY(EditDefaultsOnly, Category = "Materaials")
	class UMaterial* ExplodedMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	class UParticleSystem* FireParticles;

	UPROPERTY(EditDefaultsOnly, Category = "FX")
	class UParticleSystem* ExplosionParticles; 
	
	bool bLitOnFire;

	UFUNCTION()
	void OnHealthChanged(class UHealthComponent* HealthComponent, float Health, float HealthDelta, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);
	
	void LightOnFire();

	void Explode();

public:
	virtual void Tick(float DeltaSeconds) override;
};
