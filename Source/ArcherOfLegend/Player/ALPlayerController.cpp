// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/ALPlayerController.h"

void AALPlayerController::BeginPlay()
{

	/*Hide Cursor*/
	FInputModeGameOnly GameOnlyInputMode;
	SetInputMode(GameOnlyInputMode);  // ���� ���� �Է� ���


}
