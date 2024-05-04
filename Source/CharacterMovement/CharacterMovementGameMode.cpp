// Copyright Epic Games, Inc. All Rights Reserved.

#include "CharacterMovementGameMode.h"
#include "CharacterMovementCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACharacterMovementGameMode::ACharacterMovementGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
