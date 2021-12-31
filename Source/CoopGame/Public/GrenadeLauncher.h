// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SWeapon.h"
#include "GameFramework/Actor.h"
#include "GrenadeLauncher.generated.h"

UCLASS()
class COOPGAME_API AGrenadeLauncher : public ASWeapon
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrenadeLauncher();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void Fire() override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UParticleSystem* ExplosionEffect;
	
	class AGrenadeProjectile* ProjectileClass;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
