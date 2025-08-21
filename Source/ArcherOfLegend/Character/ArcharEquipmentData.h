// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ArcharEquipmentData.generated.h"



UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	None      UMETA(DisplayName = "None"),
	Equipped  UMETA(DisplayName = "Equipped"), // 손에 들고 있음
	Sheathed  UMETA(DisplayName = "Sheathed")  // 등에 멤
};
/**
 * 
 */
UCLASS()
class ARCHEROFLEGEND_API UArcharEquipmentData : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UArcharEquipmentData();
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equip|Offsets", Meta = (AllowPrivateAccess = "true"))
	FName WeaponComponentName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Equip|Offsets", Meta = (AllowPrivateAccess = "true"))
	FTransform WeaponOffset;


};
