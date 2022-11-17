// Fill out your copyright notice in the Description page of Project Settings.


#include "Bomb_Base.h"

#include "BaseCharacter.h"
#include "EnemyBase.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ABomb_Base::ABomb_Base()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	StaticMesh->SetSimulatePhysics(true);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	SphereComponent->SetupAttachment(StaticMesh);
	SphereComponent->SetSphereRadius(1300.0f);

	CountDown = 2.0f;
}

// Called when the game starts or when spawned
void ABomb_Base::BeginPlay()
{
	Super::BeginPlay();

	AddImpulse();
}

// Called every frame
void ABomb_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABomb_Base::AddImpulse()
{
	ABaseCharacter* Player = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	FRotator Rotation(Player->GetControlRotation());
	const FVector Impulse(5000, 5000, 500);
	FVector Direction = Impulse * UKismetMathLibrary::GetForwardVector(Rotation);

	this->StaticMesh->AddImpulse(Direction);

	GetWorldTimerManager().SetTimer(Bomb_Handle, this, &ABomb_Base::Explode, CountDown);
}

void ABomb_Base::Explode()
{
	if(GrenadeType == AGrenadeType::Frag)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionFx, StaticMesh->GetComponentLocation(), FRotator::ZeroRotator, FVector(6));
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ExplosionSound, StaticMesh->GetComponentLocation());
		OnBeginOverlap();
		Destroy();
	}
	else
	{
		UParticleSystemComponent* Emitter = UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionFx, StaticMesh->GetComponentLocation(), FRotator::ZeroRotator, FVector(6));
		UAudioComponent* Sound = UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ExplosionSound, StaticMesh->GetComponentLocation());

		FTimerDelegate Delegate;
		FTimerHandle SmokeGrenade_Handle;

		Delegate.BindLambda([&]
			{
				this->ExplosionFx->FinishDestroy();
				this->ExplosionSound->FinishDestroy();
				Destroy();
			});

		GetWorldTimerManager().SetTimer(SmokeGrenade_Handle, Delegate, 8.0f, false);
	}
}

void ABomb_Base::OnBeginOverlap()
{
	TArray<AActor*> OverlappingActors;
	SphereComponent->GetOverlappingActors(OverlappingActors);

	for (int i {0}; i < OverlappingActors.Num(); i++)
	{
		AEnemyBase* Enemy = Cast<AEnemyBase>(OverlappingActors[i]);
		ABaseCharacter* Player = Cast<ABaseCharacter>(OverlappingActors[i]);

		if (Enemy)
		{
			Enemy->TakeDamage(1.0f);
		}
		if (Player)
		{
			Player->TakeDamage(30.0);
		}
	}
	
}
