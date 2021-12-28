// Fill out your copyright notice in the Description page of Project Settings.


#include "SWeapon.h"

// Sets default values
ASWeapon::ASWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkelMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp"));
	RootComponent = SkelMeshComp;

	Range = 10000.f;
}

// Called when the game starts or when spawned
void ASWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASWeapon::Fire()
{
	AActor* OwnerActor = GetOwner();
	FVector EyeLocation;
	FRotator EyeRotation;
	OwnerActor->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector TraceEnd = EyeLocation + EyeRotation.Vector() * Range;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(OwnerActor);	// Ignoring owner collisions
	QueryParams.AddIgnoredActor(this);	// Ignoring weapon collisions
	QueryParams.bTraceComplex = true;	// Checks for every triangle while hitting target. This helps witch head shot damage
	
	FHitResult Hit;
	if(GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, ECC_Visibility, QueryParams))
	{
		// Hit!
	}
}

// Called every frame
void ASWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

