// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ALPlayerController.h"

void AALPlayerController::BeginPlay()
{

	/*Hide Cursor*/
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);  // 게임 전용 입력 모드


}
