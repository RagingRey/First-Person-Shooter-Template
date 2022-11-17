// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"

#include "BaseCharacter.h"
#include "EnemyBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	this->RootComponent = RootScene;

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	SkeletalMesh->SetupAttachment(RootScene);

	bIsFiring = false;
	FireRate = 0.1f;

	CLipSize = 45;
	Ammo = FMath::Clamp(CLipSize, 0, CLipSize);
	MaxAmmo = FMath::Clamp(135, 0, 135);

	Damage = 0.1f;
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeaponBase::SpawnAmmo()
{
	if (Ammo > 0 && ExplosionFX)
	{
		Bullet = GetWorld()->SpawnActor<AProjectileBase>(Bullet_Class, SkeletalMesh->GetSocketTransform(TEXT("Muzzle")));
		UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionFX, SkeletalMesh->GetSocketLocation("Muzzle"), SkeletalMesh->GetSocketRotation("Muzzle"))->SetWorldScale3D(FVector(0.1f, 0.1f, 0.1f));
		Ammo--;
	}
}

void AWeaponBase::SpawnRayCast()
{
	const ABaseCharacter* Player = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (!Player->bIsAiming)
		Cam = Player->Camera;
	else
		Cam = Player->AimCamera;

	FHitResult HitResult;
	const FVector RayStartLocation = SkeletalMesh->GetSocketTransform(TEXT("Muzzle")).GetLocation();
	const FVector RayEndLocation = (Cam->GetForwardVector() * 15000) + Cam->GetComponentLocation();

	if (Ammo > 0 && ExplosionFX)
	{
		GetWorld()->LineTraceSingleByChannel(HitResult, RayStartLocation, RayEndLocation, ECC_Visibility);
		DrawDebugLine(GetWorld(), SkeletalMesh->GetSocketTransform(TEXT("Muzzle")).GetLocation(), HitResult.TraceEnd, FColor::Red, true);
		UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionFX, SkeletalMesh->GetSocketLocation("Muzzle"), SkeletalMesh->GetSocketRotation("Muzzle"))->SetWorldScale3D(FVector(0.1f, 0.1f, 0.1f));
		Ammo--;

		if (AEnemyBase* Enemy = Cast<AEnemyBase>(HitResult.GetActor()); Enemy && Player->SelectedWeaponIndex == 0)
		{
			DrawDebugLine(GetWorld(), HitResult.ImpactPoint, HitResult.TraceEnd, FColor::Green, true);
			Enemy->TakeDamage(Player->PrimaryWeapon->Damage);
		}
		else if (Enemy && Player->SelectedWeaponIndex == 1)
		{
			DrawDebugLine(GetWorld(), HitResult.ImpactPoint, HitResult.TraceEnd, FColor::Green, true);
			Enemy->TakeDamage(Player->SecondaryWeapon->Damage);
		}
	}
}

void AWeaponBase::Fire()
{
	//Using RayCast
	if(!bIsFiring)
	{
		bIsFiring = true;
		GetWorldTimerManager().SetTimer(Fire_TimerHandle, this, &AWeaponBase::SpawnRayCast, FireRate, bIsFiring);
	}

	//Using Projectile
	/*if (Bullet_Class && !bIsFiring)
	{
		bIsFiring = true;
		GetWorldTimerManager().SetTimer(Fire_TimerHandle, this, &AWeaponBase::SpawnAmmo, FireRate, bIsFiring);
	}*/
}

void AWeaponBase::StopFiring()
{
	if (bIsFiring)
	{
		bIsFiring = false;
		GetWorldTimerManager().ClearTimer(Fire_TimerHandle);
	}
}

bool AWeaponBase::CanReload() const
{
	if(MaxAmmo > 0 && Ammo < CLipSize && !bIsFiring)
	{
		return true;
	}

	return false;
}

void AWeaponBase::Reload()
{
	const int Temp = CLipSize - Ammo;
	if (MaxAmmo > Temp)
	{
		MaxAmmo -= Temp;
		Ammo += Temp;
	}
	else
	{
		Ammo += MaxAmmo;
		MaxAmmo -= MaxAmmo;
	}
}
