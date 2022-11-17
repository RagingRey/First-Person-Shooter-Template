// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "Pickable_Weapon.generated.h"

UCLASS()
class FPSGAME_API APickable_Weapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickable_Weapon();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TObjectPtr<UCapsuleComponent> Capsule;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<UUserWidget> Pickup_UI_Class;
	TObjectPtr<UUserWidget> Pickup_UI;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TObjectPtr<USkeletalMeshComponent> SkeletalMesh;

	UFUNCTION()
		void OnPlayerOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int OtherBodyIndex, bool bFromSeep, const FHitResult& HitResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};