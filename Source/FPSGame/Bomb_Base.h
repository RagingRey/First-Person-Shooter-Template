// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Bomb_Base.generated.h"

UENUM(BlueprintType)
enum class AGrenadeType : uint8
{
	Frag,
	Smoke
};

UCLASS()
class FPSGAME_API ABomb_Base : public AActor
{
	GENERATED_BODY()

public:	
	// Sets default values for this actor's properties
	ABomb_Base();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		AGrenadeType GrenadeType = AGrenadeType::Frag;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TObjectPtr<UStaticMeshComponent> StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float CountDown;

	UPROPERTY(EditDefaultsOnly, Category = Utilities)
		TObjectPtr<UParticleSystem> ExplosionFx;

	UPROPERTY(EditDefaultsOnly, Category = Utilities)
		TObjectPtr<USoundBase> ExplosionSound;

	FTimerHandle Bomb_Handle;

	void AddImpulse();
	void Explode();

	void OnBeginOverlap();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
