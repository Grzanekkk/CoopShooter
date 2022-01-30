// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Camera/CameraShake.h"
#include "SWeapon.generated.h"

USTRUCT()
struct FHitScanTrace
{
	GENERATED_BODY()

	UPROPERTY()
	TEnumAsByte<EPhysicalSurface> SurfaceType;
	
	UPROPERTY()
	FVector_NetQuantize TraceTo;
};

UCLASS()
class COOPGAME_API ASWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	ASWeapon();
	
	UFUNCTION(BlueprintCallable, Category = "Actions")
	void StartFire();

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void StopFire();

	UFUNCTION(BlueprintCallable, Category = "Actions")
	void BeginReload();
	
protected:
    virtual void BeginPlay() override;

	//UFUNCTION(Server, Raliable)
	virtual void Fire();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void ServerFire();

	void Reload();
	
	void PlayFireEffects(FVector TraceStart, FVector TraceEnd) const;

	UPROPERTY(EditAnywhere, Category = "Stats")
	float ReloadTime;

	FTimerHandle TH_ReloadTimer;
	
	// Stats
	UPROPERTY(EditAnywhere, Category = "Stats")
	int LoadedAmmo;

	UPROPERTY(VisibleAnywhere, Category = "Stats")
	int AvailableAmmo;

	UPROPERTY(EditAnywhere, Category = "Stats")
	int MagazineCapacity;
	
	UPROPERTY(EditAnywhere, Category = "Stats")
	int MaxAmmo;

	UPROPERTY(EditAnywhere, Category = "Stats")
	float Range;

	UPROPERTY(EditAnywhere, Category = "Stats")
	float Damage;

	UPROPERTY(EditAnywhere, Category = "Stats")
	float HeadShotDamageMultiplier;

	UPROPERTY(EditAnywhere, Category = "Stats", meta = (ClampMin = "0.05"))
	float FireRate;			// Delay between shots

	FTimerHandle TH_FireRate;

	float LastFireTime;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<UDamageType> DamageType;

	// Mesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USkeletalMeshComponent* SkelMeshComp;

	// Particles

	void PlayImpactEffects(EPhysicalSurface SurfaceType, FVector ImpactPoint);
	
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UParticleSystem* MuzzleFlashEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UParticleSystem* DefaultImpactEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UParticleSystem* BodyImpactEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UParticleSystem* TraceEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<UCameraShakeBase> FireCameraShake;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")	// VisibleDefaultsOnly, we still show this value in the editor, but we need to set it ourself. It's helpful for designers.
	FName MuzzleSocketName;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
	FName TraceTargetName;

	UPROPERTY(ReplicatedUsing=OnRep_HitScanTrace)
	FHitScanTrace HitScanTrace;
	
	FVector TraceEndPoint;
	FVector TraceStartPoint;

	UFUNCTION()
	void OnRep_HitScanTrace();
	
};