// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"

#include "Bomb_Base.h"
#include "EnemyBase.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	this->GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	this->bUseControllerRotationPitch = false;
	this->bUseControllerRotationYaw = true;
	this->bUseControllerRotationRoll = false;

	this->GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	this->GetCharacterMovement()->bOrientRotationToMovement = true;
	this->GetCharacterMovement()->RotationRate = FRotator(0.0f, 0.0f, 500.0f);
	this->GetCharacterMovement()->JumpZVelocity = 500.0f;
	this->GetCharacterMovement()->AirControl = 0.05f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(this->GetMesh(), FName("Head"));
	Camera->bUsePawnControlRotation = true;

	GunSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun Mesh"));
	GunSkeletalMesh->SetupAttachment(this->GetMesh(), FName("Weapon_Attach"));

	AimCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Aim Down Sight Camera"));
	AimCamera->SetupAttachment(GunSkeletalMesh, FName("Muzzle"));

	bDead = false;
	bInDangerZone = false;
	bIsCrouching = false;
	bIsSprinting = false;
	bCanSprint = true;
	bCanAim = true;
	bIsReloading = false;
	bIsAiming = false;
	bIsSecondaryWeaponPicked = false;

	Health = FMath::Clamp(60.0f, 0.0f, 100.0f);
	Amour = FMath::Clamp(40.0f, 0.0f, 100.0f);

	DamageThreshold = 5;
	RegenerationThreshold = 10;

	SelectedWeaponIndex = 0;
	SelectedWeaponText = "AK47";

	FragGrenade_Count = 3;
	SmokeGrenade_Count = 3;
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if(MainHUD_Class != nullptr)
	{
		MainHUD = CreateWidget(GetWorld(), MainHUD_Class);
		MainHUD->AddToViewport();
	}

	if(!Weapon_Class.IsEmpty())
	{
		PrimaryWeapon = GetWorld()->SpawnActor<AWeaponBase>(Weapon_Class[0], FVector::ZeroVector, FRotator::ZeroRotator);
		PrimaryWeapon->AttachToComponent(this->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Weapon_Attach"));
	}

	this->GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ABaseCharacter::OnBeginOverlap);
	this->GetCapsuleComponent()->OnComponentEndOverlap.AddDynamic(this, &ABaseCharacter::OnEndOverlap);
}

// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RegenerateAmour(DeltaTime);
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Aim Down Sight", IE_Pressed, this, &ABaseCharacter::Aim);
	PlayerInputComponent->BindAction("Aim Down Sight", IE_Released, this, &ABaseCharacter::StopAiming);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ABaseCharacter::Fire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ABaseCharacter::StopFiring);

	PlayerInputComponent->BindAction("Frag Grenade", IE_Pressed, this, &ABaseCharacter::SpawnFragGrenade);
	PlayerInputComponent->BindAction("Smoke Grenade", IE_Pressed, this, &ABaseCharacter::SpawnSmokeGrenade);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ABaseCharacter::Crouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ABaseCharacter::StopCrouching);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ABaseCharacter::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ABaseCharacter::StopSprinting);

	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ABaseCharacter::Reload);

	PlayerInputComponent->BindAction("Select Primary Weapon", IE_Pressed, this, &ABaseCharacter::SelectPrimaryWeapon);
	PlayerInputComponent->BindAction("Select Secondary Weapon", IE_Pressed, this, &ABaseCharacter::SelectSecondaryWeapon);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &ABaseCharacter::MoveRight);
}

void ABaseCharacter::MoveForward(float Axis)
{
	if(!bDead)
	{
		const FRotator Rotation = this->Controller->GetControlRotation();
		const FRotator YawRotation = FRotator(0.0f, Rotation.Yaw, 0.0f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Axis);
	}
}

void ABaseCharacter::MoveRight(float Axis)
{
	if (!bDead)
	{
		const FRotator Rotation = this->Controller->GetControlRotation();
		const FRotator YawRotation = FRotator(0.0f, Rotation.Yaw, 0.0f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Axis);
	}
}

void ABaseCharacter::RegenerateAmour(float DeltaTime)
{
	if(Amour < 100.0f && !bInDangerZone)
	{
		Amour += DeltaTime * RegenerationThreshold;
	}
}

void ABaseCharacter::TakeDamage()
{
	if (Amour > 0.0f)
		Amour -= DamageThreshold;
	else
		Health -= DamageThreshold;

	if (DamageVignetteHUD_Class != nullptr)
	{
		DamageVignetteHUD = CreateWidget(GetWorld(), DamageVignetteHUD_Class);
		DamageVignetteHUD->AddToViewport();
	}
}

void ABaseCharacter::TakeDamage(float Damage)
{
	if(Amour > 0.0f)
		Amour -= Damage;
	else
		Health -= Damage;

	if(DamageVignetteHUD_Class != nullptr)
	{
		DamageVignetteHUD = CreateWidget(GetWorld(), DamageVignetteHUD_Class);
		DamageVignetteHUD->AddToViewport();
	}
}

void ABaseCharacter::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int OtherBodyIndex, bool bFromSeep, const FHitResult& HitResult)
{
	if (OtherActor->ActorHasTag("DangerZone") && !bDead)
	{
		bInDangerZone = true;
		GetWorldTimerManager().SetTimer(TakeDamage_TimeHandler, this, &ABaseCharacter::TakeDamage, 0.5f, bInDangerZone);
	}
}

void ABaseCharacter::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int OtherBodyIndex)
{
	if(OtherActor->ActorHasTag("DangerZone") && !bDead)
	{
		bInDangerZone = false;
		GetWorldTimerManager().ClearTimer(TakeDamage_TimeHandler);
	}
}
void ABaseCharacter::SpawnAmmo()
{
	if(SelectedWeaponIndex == 0)
	{
		PrimaryWeapon->SpawnAmmo();
	}
	else if (SelectedWeaponIndex == 1)
	{
		SecondaryWeapon->SpawnAmmo();
	}
	
}

void ABaseCharacter::SpawnFragGrenade()
{
	if(FragGrenade_Class && FragGrenade_Count > 0)
	{
		FragGrenade_Count--;
		const FRotator GrenadeSpawnRotation = this->GetControlRotation();
		const FVector GrenadeSpawnLocation = this->GetActorLocation() + GrenadeSpawnRotation.RotateVector(FVector(300, 0, 0));
		ABomb_Base* Bomb = GetWorld()->SpawnActor<ABomb_Base>(FragGrenade_Class, GrenadeSpawnLocation, this->GetActorRotation());
	}
}

void ABaseCharacter::SpawnSmokeGrenade()
{
	if (SmokeGrenade_Class && SmokeGrenade_Count > 0)
	{
		SmokeGrenade_Count--;
		const FRotator GrenadeSpawnRotation = this->GetControlRotation();
		const FVector GrenadeSpawnLocation = this->GetActorLocation() + GrenadeSpawnRotation.RotateVector(FVector(300, 0, 0));
		ABomb_Base* Bomb = GetWorld()->SpawnActor<ABomb_Base>(SmokeGrenade_Class, GrenadeSpawnLocation, this->GetActorRotation());
	}
}

void ABaseCharacter::Fire()
{
	if (!Weapon_Class.IsEmpty() && !bIsSprinting && !bIsReloading && SelectedWeaponIndex == 0)
	{
		PrimaryWeapon->Fire();
	}
	else if(!Weapon_Class.IsEmpty() && !bIsSprinting && !bIsReloading && SelectedWeaponIndex == 1)
	{
		SecondaryWeapon->Fire();
	}
}

void ABaseCharacter::StopFiring()
{
	if(SelectedWeaponIndex == 0)
	{
		PrimaryWeapon->StopFiring();
	}
	else if (SelectedWeaponIndex == 1)
	{
		SecondaryWeapon->StopFiring();
	}
}

void ABaseCharacter::Aim()
{
	if(bCanAim)
	{
		StopSprinting();

		Camera->Deactivate();
		AimCamera->Activate();
		this->GetCharacterMovement()->MaxWalkSpeed = 250.0f;
		bIsAiming = true;
	}
}

void ABaseCharacter::StopAiming()
{
	Camera->Activate();
	AimCamera->Deactivate();
	this->GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	bIsAiming = false;
}

void ABaseCharacter::Reload()
{
	StopAiming();

	if (SelectedWeaponIndex == 0 && PrimaryWeapon->CanReload())
	{
		bCanAim = false;
		bIsReloading = true;
	}
	if (SelectedWeaponIndex == 1 && SecondaryWeapon->CanReload())
	{
		bCanAim = false;
		bIsReloading = true;
	}
	
	FTimerHandle Reload_TimeHandle;
	FTimerDelegate Delegate;

	Delegate.BindLambda([&]
		{
			if (SelectedWeaponIndex == 0 && bIsReloading)
			{
				PrimaryWeapon->Reload();
				UE_LOG(LogTemp, Log, TEXT("PRan"));
			}
			if (SelectedWeaponIndex == 1 && bIsReloading)
			{
				SecondaryWeapon->Reload();
				UE_LOG(LogTemp, Log, TEXT("SRan"));
			}

			bIsReloading = false;
			bCanAim = true;
		});
	GetWorldTimerManager().SetTimer(Reload_TimeHandle, Delegate, 3.6f, false);
}

void ABaseCharacter::Crouch()
{
	if (!bIsCrouching)
	{
		StopSprinting();

		bIsCrouching = true;
		this->GetCharacterMovement()->MaxWalkSpeed = 250.0f;
		ACharacter::Crouch();
	}
}

void ABaseCharacter::StopCrouching()
{
	if (bIsCrouching)
	{
		bIsCrouching = false;
		this->GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
		ACharacter::UnCrouch();
	}
		
}

void ABaseCharacter::Sprint()
{
	if (!bIsSprinting && bCanSprint && !PrimaryWeapon->bIsFiring)
	{
		StopAiming();
		StopCrouching();

		bIsSprinting = true;
		this->GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
	}
}

void ABaseCharacter::StopSprinting()
{
	if (bIsSprinting)
	{
		bIsSprinting = false;
		this->GetCharacterMovement()->MaxWalkSpeed = MaxWalkSpeed;
	}
}

void ABaseCharacter::SelectPrimaryWeapon()
{
	if (bIsSecondaryWeaponPicked && Weapon_Class[0] && SelectedWeaponIndex != 0)
	{
		SelectedWeaponIndex = 0;
		SelectedWeaponText = "AK47";
		PrimaryWeapon->SkeletalMesh->SetVisibility(true);
		SecondaryWeapon->SkeletalMesh->SetVisibility(false);
	}
}

void ABaseCharacter::SelectSecondaryWeapon()
{
	if(!SecondaryWeapon && bIsSecondaryWeaponPicked && Weapon_Class[1] && SelectedWeaponIndex != 1)
	{
		SelectedWeaponIndex = 1;
		SelectedWeaponText = "M4A1";
		PrimaryWeapon->SkeletalMesh->SetVisibility(false);
		SecondaryWeapon = GetWorld()->SpawnActor<AWeaponBase>(Weapon_Class[1], FVector::ZeroVector, FRotator::ZeroRotator);
		SecondaryWeapon->AttachToComponent(this->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("Weapon_Attach"));
	}
	else if(SecondaryWeapon && bIsSecondaryWeaponPicked && Weapon_Class[1] && SelectedWeaponIndex != 1)
	{
		SelectedWeaponIndex = 1;
		SelectedWeaponText = "M4A1";
		PrimaryWeapon->SkeletalMesh->SetVisibility(false);
		SecondaryWeapon->SkeletalMesh->SetVisibility(true);
	}
}