// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerController_Base.generated.h"

/**
 * 
 */
UCLASS()
class FPSGAME_API APlayerController_Base : public APlayerController
{
	GENERATED_BODY()

public:
	APlayerController_Base();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=GameProperty)
		bool bIsGamePaused;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Utilities)
		TSubclassOf<UUserWidget> Pause_UI_Class;
	TObjectPtr<UUserWidget> Pause_UI;

	void PauseGame();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void SetupInputComponent();
};
