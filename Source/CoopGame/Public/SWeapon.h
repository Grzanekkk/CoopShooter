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
	virtual void Fire();

protected:
	void PlayFireEffects(FVector TraceStart, FVector TraceEndPoint) const;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USkeletalMeshComponent* SkelMeshComp;

	UPROPERTY(EditAnywhere, Category = "Params")
	float Range;

	UPROPERTY(EditAnywhere, Category = "Stats")
	float Damage;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UParticleSystem* MuzzleFlashEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UParticleSystem* ImpctEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UParticleSystem* TraceEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<UCameraShakeBase> FireCamerShake;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")	// VisibleDefaultsOnly, we still show this value in the editor, but we need to set it ourself. It's helpful for designers.
	FName MuzzleSocketName;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
	FName TraceTargetName;
};