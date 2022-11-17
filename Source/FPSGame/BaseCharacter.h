// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include <iostream>

#include "Bomb_Base.h"
#include "BaseCharacter.generated.h"

UCLASS()
class FPSGAME_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
		TObjectPtr<USkeletalMeshComponent> GunSkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		TObjectPtr<UCameraComponent> AimCamera;

	bool bDead;
	bool bInDangerZone;

	float MaxWalkSpeed = 500.0f;

	void MoveForward(float Axis);
	void MoveRight(float Axis);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Utilities)
		float Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Utilities)
		float Amour;

	UPROPERTY(EditAnywhere, Category = Utilities)
		int DamageThreshold;

	UPROPERTY(EditAnywhere, Category = Utilities)
		int RegenerationThreshold;

	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<UUserWidget> MainHUD_Class;
	TObjectPtr<UUserWidget> MainHUD;

	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<UUserWidget> DamageVignetteHUD_Class;
	TObjectPtr<UUserWidget> DamageVignetteHUD;

	UPROPERTY(EditAnywhere, Category = "Weapons")
		TArray<TSubclassOf<AWeaponBase>> Weapon_Class;
	UPROPERTY(BlueprintReadOnly)
		TObjectPtr<AWeaponBase> PrimaryWeapon;
	UPROPERTY(BlueprintReadOnly)
		TObjectPtr<AWeaponBase> SecondaryWeapon;

	UPROPERTY(EditAnywhere, Category = "Weapons")
		TSubclassOf<ABomb_Base> FragGrenade_Class;

	UPROPERTY(EditAnywhere, Category = "Weapons")
		TSubclassOf<ABomb_Base> SmokeGrenade_Class;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Utilities)
		int FragGrenade_Count;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Utilities)
		int SmokeGrenade_Count;


	FTimerHandle TakeDamage_TimeHandler;
	void TakeDamage();
	void TakeDamage(float Damage);
	void RegenerateAmour(float DeltaTime);

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int OtherBodyIndex, bool bFromSeep, const FHitResult& HitResult);
	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int OtherBodyIndex);

	void SpawnAmmo();
	void SpawnFragGrenade();
	void SpawnSmokeGrenade();
	void Fire();
	void StopFiring();
	void Aim();
	void StopAiming();
	void Reload();
	void SelectPrimaryWeapon();
	void SelectSecondaryWeapon();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		FString SelectedWeaponText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int SelectedWeaponIndex;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bIsReloading;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bIsCrouching;

	void Crouch();
	void StopCrouching();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bCanSprint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bCanAim;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bIsAiming;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bIsSprinting;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bIsSecondaryWeaponPicked;

	void Sprint();
	void StopSprinting();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
