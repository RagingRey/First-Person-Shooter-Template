// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"

#include "BaseCharacter.h"
#include "NavigationSystem.h"
#include "AI/NavigationSystemBase.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensing"));
	PawnSensing->SightRadius = 1500;
	PawnSensing->SetPeripheralVisionAngle(45.0f);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	SphereComponent->SetupAttachment(this->GetMesh(), FName("AttackCollision"));
	SphereComponent->SetWorldScale3D(FVector(0.15f));
	SphereComponent->SetCollisionObjectType(ECC_WorldDynamic);
	SphereComponent->SetCollisionResponseToAllChannels(ECR_Overlap);

	this->GetCharacterMovement()->MaxWalkSpeed = 250.0f;
	this->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	bSeenPlayer = false;
	
	Health = 1.0f;
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	AIController = Cast<AAIController>(this->GetController());

	this->OnActorBeginOverlap.AddDynamic(this, &AEnemyBase::OnBeginOverlap);
	PawnSensing->OnSeePawn.AddDynamic(this, &AEnemyBase::OnSeePawn);
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemyBase::OnAttackOverlap);

	GetWorldTimerManager().SetTimer(Patrol_Handle, this, &AEnemyBase::Patrol, 5.0f, true);
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyBase::OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor->ActorHasTag("AK47 Bullet"))
	{
		TakeDamage(0.1f);
	}
	else if (OtherActor->ActorHasTag("M4A1 Bullet"))
	{
		TakeDamage(0.2f);
	}
}

void AEnemyBase::OnSeePawn(APawn* Pawn)
{
	if (Pawn == Cast<ABaseCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		bSeenPlayer = true;
		this->GetCharacterMovement()->MaxWalkSpeed = 250.0f;
		AIController->MoveToActor(Pawn);
		Attack();
	}
}

void AEnemyBase::Patrol()
{
	if(!bSeenPlayer)
	{
		this->GetCharacterMovement()->MaxWalkSpeed = 125.0f;
		AIController->MoveTo(UNavigationSystemV1::GetRandomReachablePointInRadius(this, this->GetActorLocation(), 500.0f));
	}
}

void AEnemyBase::Attack()
{
	if(!bIsAttacking)
	{
		bIsAttacking = true;

		FTimerHandle Attack_Handle;
		FTimerDelegate Delegate;

		Delegate.BindLambda([&]
			{
				bIsAttacking = false;
				AIController->MoveToActor(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
			});

		GetWorldTimerManager().SetTimer(Attack_Handle, Delegate, 3.0f, false);
	}
}

void AEnemyBase::TakeDamage(float Damage)
{
	if (Health > 0)
	{
		bSeenPlayer = true;
		Health -= Damage;

		if (HitMarkerHUD_Class)
		{
			HitMarkerHUD = CreateWidget(GetWorld(), HitMarkerHUD_Class);
			HitMarkerHUD->AddToViewport();
		}

		AIController->MoveToActor(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	}

	if(!bDead && Health <= 0)
	{
		bDead = true;
		Death();
	}
}

void AEnemyBase::Death()
{
	if (KillFeedHUD_Class)
	{
		KillFeedHUD = CreateWidget(GetWorld(), KillFeedHUD_Class);
		KillFeedHUD->AddToViewport();
	}

	this->GetMesh()->SetSimulatePhysics(true);
}

void AEnemyBase::OnAttackOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor == Cast<ABaseCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0)))
	{
		ABaseCharacter* Player = Cast<ABaseCharacter>(OtherActor);
		Player->TakeDamage(5.0f);
	}
}
