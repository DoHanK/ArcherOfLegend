// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/ALCharacterBase.h"
#include "InputActionValue.h"
#include "Character/ArcharEquipmentData.h"
#include "ALCharacterPlayer.generated.h"

/**
 * 
 */
UCLASS()
class ARCHEROFLEGEND_API AALCharacterPlayer : public AALCharacterBase
{
	GENERATED_BODY()
	
public:
	AALCharacterPlayer();

public:
	 virtual void BeginPlay() override;
	

//Camera Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent>	FollowCamera;

// Input Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ZoomAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> WeaponToggleAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MouseAttackAction;


	void Move(const FInputActionValue& Value);
	void MouseAttack(const FInputActionValue& value);
	void EndMouseAttackAnimation(UAnimMontage* TargetMontage, bool IsProperlyEnded);

	void Look(const FInputActionValue& Value);
	void Zoom(const FInputActionValue& Value);	/*Camera ZoomIn And ZoomOut*/
	void WeaponToggle(const FInputActionValue& Value);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	//UpperAnimation 
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AnimationControl)
	uint8 busedupper : 1;

	void SetWeapon(const EWeaponState& state);
	void EndWeaponAnimation(UAnimMontage* TargetMontage, bool IsProperlyEnded);

	UFUNCTION()
	void SwapWeaponHand(int step);
	void PauseBowLine();

	//Attack
	void fireAllow();

};
