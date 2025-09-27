// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Allow.generated.h"

UCLASS()
class ARCHEROFLEGEND_API AAllow : public AActor
{
	GENERATED_BODY()
	
public:	

	AAllow();

public:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	TObjectPtr<class UStaticMeshComponent> AllowMeshcomponent;


};
