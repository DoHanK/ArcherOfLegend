// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMode/ALGameMode.h"
#include "PLayer/ALPlayerController.h"

AALGameMode::AALGameMode()
{
	/*Set Character*/
	//static ConstructorHelpers::FClassFinder<APawn> CharacterRef(TEXT(""));



	
	/*Set Controller*/
	PlayerControllerClass = AALPlayerController::StaticClass();

}
