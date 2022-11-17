// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPSGameGameMode.h"
#include "FPSGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFPSGameGameMode::AFPSGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
