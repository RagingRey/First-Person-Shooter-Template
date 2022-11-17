// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"
#include "EnemyBase.generated.h"

UCLASS()
class FPSGAME_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TObjectPtr<UPawnSensingComponent> PawnSensing;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TObjectPtr<USphereComponent> SphereComponent;

	TObjectPtr<AAIController> AIController;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		float Health;

	FTimerHandle Patrol_Handle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bSeenPlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool bIsAttacking;

	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<UUserWidget> HitMarkerHUD_Class;
	TObjectPtr<UUserWidget> HitMarkerHUD;

	UPROPERTY(EditAnywhere, Category = "UI")
		TSubclassOf<UUserWidget> KillFeedHUD_Class;
	TObjectPtr<UUserWidget> KillFeedHUD;

	bool bDead = false;

	void Patrol();
	void Attack();
	void TakeDamage(float Damage);
	void Death();

	UFUNCTION()
		void OnAttackOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnBeginOverlap(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
		void OnSeePawn(APawn* Pawn);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
