// Copyright Epic Games, Inc. All Rights Reserved.

#include "OriginGameMode.h"
#include "OriginCharacter.h"
#include "UObject/ConstructorHelpers.h"

AOriginGameMode::AOriginGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Player/PlayerBP"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
