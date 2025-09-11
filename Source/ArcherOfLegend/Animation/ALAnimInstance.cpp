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