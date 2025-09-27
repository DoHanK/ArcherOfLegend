// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/ALAnimInstance.h"
#include "GameFramework/Character.h"
#include "Character/ALCharacterPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

UALAnimInstance::UALAnimInstance()
{
	MovingThreshould = 3.0f;
	JumpingThreshould = 100.0f;
	UpperBlendWeight = 0.0f;
}

void UALAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner  = Cast<ACharacter>(GetOwningActor());
	if (Owner) {
	
		Movement = Owner->GetCharacterMovement();
	}


}

void UALAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		bIsIdle = GroundSpeed < MovingThreshould;
		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshould);
	}



}

float UALAnimInstance::MonoMontage_Play(UAnimMontage* MontageToPlay, float InPlayRate, EMontagePlayReturnType ReturnValueType, float InTimeToStartMontageAt, bool bStopAllMontages)
{

	// 이미 어떤 몽타주라도 실행 중이면 실행 안 함
	if (IsAnyMontagePlaying())
	{
		return -1.f; // 실행 실패 시 -1 반환
	}

	return Montage_Play(MontageToPlay, InPlayRate, ReturnValueType , InTimeToStartMontageAt, bStopAllMontages);
	
}


void UALAnimInstance::SwapFirstWeaponHand() {

	AALCharacterPlayer* player = Cast<AALCharacterPlayer>(GetOwningActor());
	if (player) {
		player->SwapWeaponHand(1);
	}
}

void UALAnimInstance::SwapSecondWeaponHand() {

	AALCharacterPlayer* player = Cast<AALCharacterPlayer>(GetOwningActor());
	if (player) {
		player->SwapWeaponHand(2);
	}
}

void UALAnimInstance::OnBowHold()
{

	if (GetBowState() == EBowState::None) {
		SetBowState(EBowState::StartPullBack);
	}
	else if (GetBowState() == EBowState::StartPullBack) {

		SetBowState(EBowState::EndPullBack);
		Montage_Pause();
		//BowLing Animation
		AALCharacterPlayer* player = Cast<AALCharacterPlayer>(GetOwningActor());
		if (player) {
			player->PauseBowLine();
		}

	}


}
