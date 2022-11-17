// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoBase.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
AAmmoBase::AAmmoBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMeshComponent->SetCollisionProfileName(TEXT("OverlapAll"));
}

// Called when the game starts or when spawned
void AAmmoBase::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	this->OnActorBeginOverlap.AddDynamic(this, &AAmmoBase::OnBeginOverlap);
}

// Called every frame
void AAmmoBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAmmoBase::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if(OtherActor == Character)
	{
		Character->PrimaryWeapon->MaxAmmo += 50;
		Destroy();
	}
}