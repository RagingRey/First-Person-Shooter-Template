// Fill out your copyright notice in the Description page of Project Settings.


#include "Base_GameMode.h"

#include "Kismet/GameplayStatics.h"

ABase_GameMode::ABase_GameMode()
{
	Seconds = 5;
	Minutes = 1;
}

void ABase_GameMode::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(Timer_Handle, this, &ABase_GameMode::UpdateTimer, 1.0f, true);
}

void ABase_GameMode::UpdateTimer()
{
	if (Minutes < 0)
	{
		Seconds = 0;
		UGameplayStatics::SetGamePaused(GetWorld(), true);
	}

	if (Seconds < 1)
	{
		Seconds = 60;
		Minutes--;
	}

	Seconds--;
}