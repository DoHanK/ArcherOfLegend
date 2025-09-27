// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/ALCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "Character/ArcharEquipmentData.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AALCharacterBase::AALCharacterBase()
{
	
	//World Character Basic Setting
	
	//Pawn
 	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	//Capsule
	GetCapsuleComponent()->InitCapsuleSize(30.f,100.f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

	//Movement
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f,500.0f,0.0f);
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	//Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));


	//Equipment
	WeaponState = EWeaponState::Equipped; /*손에 집은 상태로 시작*/
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Weapon"));


}


