// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Allow.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AAllow::AAllow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	 AllowMeshcomponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisibleAllow"));

	 ConstructorHelpers::FObjectFinder<UStaticMesh> ArrowMesh(TEXT("/Script/Engine.StaticMesh'/Game/ElfSelena/BaseMesh/Separate/SM_arrow.SM_arrow'"));

	 if (ArrowMesh.Object) {
		 AllowMeshcomponent->SetStaticMesh(ArrowMesh.Object);
	 }

	 RootComponent = AllowMeshcomponent;
	 AllowMeshcomponent->SetCollisionProfileName(TEXT("NoCollision"));

}



