// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ALAnimInstance.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EBowState : uint8
{
	None  UMETA(DisplayName = "None"),
	StartPullBack      UMETA(DisplayName = "StartPullBack"),
	EndPullBack      UMETA(DisplayName = "EndPullBack")

};

UCLASS()
class ARCHEROFLEGEND_API UALAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UALAnimInstance();
	
	float MonoMontage_Play(UAnimMontage* MontageToPlay, float InPlayRate = 1.f, EMontagePlayReturnType ReturnValueType = EMontagePlayReturnType::MontageLength, float InTimeToStartMontageAt = 0.f, bool bStopAllMontages = true);

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class ACharacter> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<class UCharacterMovementComponent> Movement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	FVector Velocity;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	float GroundSpeed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsIdle : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	float MovingThreshould;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsFalling : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsJumping : 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	float JumpingThreshould;
	//HandWeapon
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	uint8 IsHandWeapon : 1;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	float UpperBlendWeight;



public:
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SwapFirstWeaponHand();
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SwapSecondWeaponHand();

	void SetWeaponState(bool bHandWeapon){ IsHandWeapon = bHandWeapon;}
	void SetUpperBlendWeight(float weight) { UpperBlendWeight = weight; }
	float GetUpperBlendWeight(){return UpperBlendWeight;}
	//Attack
public: 

	//Attack
	EBowState bHoldBow;
	EBowState GetBowState(){return bHoldBow;}
	void SetBowState(EBowState state){ bHoldBow  = state;}
	UFUNCTION(BlueprintCallable, Category = "Attck")
	void OnBowHold();


};
