// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController_Base.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"


APlayerController_Base::APlayerController_Base()
{
	bIsGamePaused = false;
}

void APlayerController_Base::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerController_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

void APlayerController_Base::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("Pause", IE_Pressed, this, &APlayerController_Base::PauseGame);
}



void APlayerController_Base::PauseGame()
{
	const FInputModeGameAndUI Paused;
	const FInputModeGameOnly NotPaused;
	bool bIsCreated = false;

	if(bIsCreated && !bIsGamePaused)
	{
		Pause_UI->AddToViewport();
		SetShowMouseCursor(true);
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		bIsGamePaused = true;
		SetInputMode(Paused);
	}
	else if(Pause_UI_Class && !bIsGamePaused)
	{
		Pause_UI = CreateWidget(this, Pause_UI_Class);
		Pause_UI->AddToViewport();
		SetShowMouseCursor(true);
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		bIsCreated = true;
		bIsGamePaused = true;
		SetInputMode(Paused);
	}
	else if(bIsGamePaused)
	{
		Pause_UI->RemoveFromViewport();
		SetShowMouseCursor(false);
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		bIsGamePaused = false;
		SetInputMode(NotPaused);
	}
}
