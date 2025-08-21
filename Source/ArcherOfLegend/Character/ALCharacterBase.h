// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Character/ArcharEquipmentData.h"
#include "ALCharacterBase.generated.h"



UCLASS()
class ARCHEROFLEGEND_API AALCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AALCharacterBase();

//Equipment Section
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Equipment, Meta = (AllowPrivateAccess = "true"))
	EWeaponState WeaponState;

	UPROPERTY(EditAnywhere, Category = "EquipControl", Meta = (AllowPrivateAccess = "true"))
	TMap< EWeaponState, class UArcharEquipmentData* > WeaponOffsetManager;

};
