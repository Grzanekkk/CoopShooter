// Fill out your copyright notice in the Description page of Project Settings.


#include "GrenadeLauncher.h"

// Sets default values
AGrenadeLauncher::AGrenadeLauncher()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SkelMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	RootComponent = SkelMeshComp;

	MuzzleSocketName = "MuzzleSocket";
}

// Called when the game starts or when spawned
void AGrenadeLauncher::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void AGrenadeLauncher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGrenadeLauncher::Fire()
{
	
}
