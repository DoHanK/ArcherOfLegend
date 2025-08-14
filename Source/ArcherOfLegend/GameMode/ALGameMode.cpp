// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMode/ALGameMode.h"
#include "PLayer/ALPlayerController.h"

AALGameMode::AALGameMode()
{
	/*Set Character*/
	static ConstructorHelpers::FClassFinder<APawn> PlayerCharacterRef(TEXT("/Script/CoreUObject.Class'/Script/ArcherOfLegend.ALCharacterPlayer'"));
	if (PlayerCharacterRef.Class) {
		DefaultPawnClass = PlayerCharacterRef.Class;
	}

	
	/*Set Controller*/
	PlayerControllerClass = AALPlayerController::StaticClass();

}
