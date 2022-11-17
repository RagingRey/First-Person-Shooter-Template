// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Base_GameMode.generated.h"

/**
 * 
 */
UCLASS()
class FPSGAME_API ABase_GameMode : public AGameMode
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABase_GameMode();

	FTimerHandle Timer_Handle;

	UPROPERTY(BlueprintReadOnly)
		int Minutes;
	UPROPERTY(BlueprintReadOnly)
		int Seconds;

	void UpdateTimer();

public:
	// Called every frame
	virtual void BeginPlay() override;
};
