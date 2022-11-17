// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.generated.h"

UCLASS()
class FPSGAME_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TObjectPtr<USceneComponent> RootScene;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TObjectPtr<USkeletalMeshComponent> SkeletalMesh;

	UPROPERTY(EditAnywhere, Category = Ammo)
		TSubclassOf<AProjectileBase> Bullet_Class;
	TObjectPtr<AProjectileBase> Bullet;

	UPROPERTY(EditAnywhere, Category = Utilities)
		TObjectPtr<UParticleSystem> ExplosionFX;

	UPROPERTY(BlueprintReadOnly, Category = Ammo)
		bool bIsFiring;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ammo)
		float FireRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ammo)
		int Ammo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ammo)
		int MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ammo)
		int CLipSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Utilities)
		float Damage;

	FTimerHandle Fire_TimerHandle;

	void SpawnAmmo();
	void SpawnRayCast();
	void Fire();
	void StopFiring();
	bool CanReload() const;
	void Reload();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UCameraComponent* Cam;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
