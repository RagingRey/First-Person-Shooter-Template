// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickable_Weapon.h"

#include "BaseCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APickable_Weapon::APickable_Weapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	SkeletalMesh->SetupAttachment(Capsule);
}

// Called when the game starts or when spawned
void APickable_Weapon::BeginPlay()
{
	Super::BeginPlay();

	Capsule->OnComponentBeginOverlap.AddDynamic(this, &APickable_Weapon::OnPlayerOverlap);
}

// Called every frame
void APickable_Weapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APickable_Weapon::OnPlayerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int OtherBodyIndex, bool bFromSeep, const FHitResult& HitResult)
{
	if (OtherActor == UGameplayStatics::GetPlayerPawn(GetWorld(), 0))
	{
		ABaseCharacter* Player = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		Player->bIsSecondaryWeaponPicked = true;

		if(Pickup_UI_Class)
		{
			Pickup_UI = CreateWidget(GetWorld(), Pickup_UI_Class);
			Pickup_UI->AddToViewport();
		}

		Destroy();
	}
}