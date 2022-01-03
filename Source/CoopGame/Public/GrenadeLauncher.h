// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GrenadeProjectile.h"
#include "SWeapon.h"
#include "GrenadeLauncher.generated.h"

UCLASS()
class COOPGAME_API AGrenadeLauncher : public ASWeapon
{
	GENERATED_BODY()
	
public:	
	AGrenadeLauncher();

protected:
	virtual void Fire() override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UParticleSystem* ExplosionEffect;

	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	USoundCue* LaunchCue;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<AGrenadeProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = "Stats")
	float ExplosionRadius;

	UPROPERTY(EditAnywhere, Category = "Stats")
	float ExplosionDelay;
};