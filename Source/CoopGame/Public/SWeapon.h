// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWeapon.generated.h"

UCLASS()
class COOPGAME_API ASWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void Fire();

	void PlayFiringEffects();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USkeletalMeshComponent* SkelMeshComp;

	UPROPERTY(EditAnywhere, Category = "Params")
	float Range;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<UDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UParticleSystem* MuzzleFlashEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UParticleSystem* ImpctEffect;

	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")	// VisibleDefaultsOnly, we still show this value in the editor, but we need to set it ourself. It's helpful for designers.
	FName MuzzleSocketName;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};