// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GrenadeProjectile.generated.h"

UCLASS()
class COOPGAME_API AGrenadeProjectile : public AActor
{
	GENERATED_BODY()
	
public:
	virtual void Tick(float DeltaSeconds) override;
	
	AGrenadeProjectile();
	
	FORCEINLINE void SetDamage(float DamageVaule) { if(DamageVaule > 0 && DamageVaule < 100000) { ExplosionDamage = DamageVaule; } }

	FORCEINLINE void SetExplosionRadius(float Radius) { ExplosionRadius = Radius; }

	FORCEINLINE void SetExplosionDelay(float Delay) { ExplosionDelay = Delay; } 

	FORCEINLINE void SetExplosionParticles(UParticleSystem* _ExplosionParticles) { ExplosionParticles = _ExplosionParticles; }
	
protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void Explode();

	UPROPERTY(VisibleAnywhere, Category = "Damage")
	TSubclassOf<UDamageType> DamageType;
	
	UPROPERTY(VisibleAnywhere, Category = "Damage")
	float ExplosionRadius;

	UPROPERTY(VisibleAnywhere, Category = "Damage")
	float ExplosionDamage;

	UPROPERTY(VisibleAnywhere, Category = "Damage")
	float DefaultDamageValue = 50.f;

	UPROPERTY(VisibleAnywhere, Category = "Damage")
	float ExplosionDelay;

	UPROPERTY(VisibleAnywhere, Category = "FX")
	UParticleSystem* ExplosionParticles;
	
	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	class USoundCue* ExplosionCue;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components")
	class USphereComponent* CollisionComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UMeshComponent* MeshComp;

public:	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
