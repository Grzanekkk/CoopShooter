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
	AGrenadeProjectile();
	
	void SetDamage(float DamageVaule);

	void SetExplosionParticles(UParticleSystem* _ExplosionPartilces);
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
	float DefaultDamageVaule = 50.f;

	UPROPERTY(VisibleAnywhere, Category = "Damage")
	float ExplosionDelay;

	UPROPERTY(VisibleAnywhere, Category = "FX")
	UParticleSystem* ExplosionParticles;

	FTimerHandle TH_ExplosionDelay;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category= "Components")
	class USphereComponent* CollisionComp;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UMeshComponent* MeshComp;

public:	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
