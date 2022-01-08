// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

UCLASS()
class COOPGAME_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void StartFire();
	
	void StopFire();

	void Reload();

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	class ASWeapon* CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<ASWeapon> StarterWeaponClass;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	FName WeaponSocketName;

	void MoveForward(float Value);

	void MoveRight(float Value);
	
	void BeginCrouch();
	
	void EndCrouch();

	void BeginZoom();

	void EndZoom();

	virtual void Jump() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")	// We can only edit already created component, we cant assign new one in Blueprint
	class UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class USpringArmComponent* SpringArmComp;

	bool bWantsToZoom;
	
	float DefaultFOV;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float ZoomedFOV;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float ZoomInterpSpeed;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual FVector GetPawnViewLocation() const override; 

};
