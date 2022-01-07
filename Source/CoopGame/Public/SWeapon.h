// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraShake.h"
#include "SWeapon.generated.h"


UCLASS()
class COOPGAME_API ASWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ASWeapon();
	
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StartFire();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void StopFire();
	
protected:
    virtual void BeginPlay() override;

	virtual void Fire();
	
	void PlayFireEffects(FVector TraceStart, FVector TraceEndPoint) const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USkeletalMeshComponent* SkelMeshComp;

	UPROPERTY(EditAnywhere, Category = "Stats")
	float Range;

	UPROPERTY(EditAnywhere, Category = "Stats")
	float Damage;

	UPROPERTY(EditAnywhere, Category = "Stats")
	float HeadShotDamageMultipler;

	UPROPERTY(EditAnywhere, Category = "Stats")
	float FireRate;			// Delay between shots

	FTimerHandle TH_FireRate;

	float LastFireTime;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UParticleSystem* MuzzleFlashEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UParticleSystem* DefaultImpctEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UParticleSystem* BodyImpctEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UParticleSystem* TraceEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<UCameraShakeBase> FireCamerShake;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")	// VisibleDefaultsOnly, we still show this value in the editor, but we need to set it ourself. It's helpful for designers.
	FName MuzzleSocketName;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
	FName TraceTargetName;
};