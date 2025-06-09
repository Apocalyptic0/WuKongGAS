// Copyright Epic Games, Inc. All Rights Reserved.

#include "WuKongGASGameMode.h"
#include "WuKongGASCharacter.h"
#include "UObject/ConstructorHelpers.h"

AWuKongGASGameMode::AWuKongGASGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
