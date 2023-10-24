// Copyright Epic Games, Inc. All Rights Reserved.

#include "LiveWireRechargedGameMode.h"
#include "LiveWireRechargedCharacter.h"
#include "UObject/ConstructorHelpers.h"

ALiveWireRechargedGameMode::ALiveWireRechargedGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
