// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ALCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"

AALCharacterPlayer::AALCharacterPlayer()
{
	
	/*Load Character*/
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> PlayerMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ElfSelena/BaseMesh/SK_ElfSelena.SK_ElfSelena'"));
	if (PlayerMeshRef.Object) {
		GetMesh()->SetSkeletalMesh(PlayerMeshRef.Object);
	}

	
	//Camera Setting
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 700.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom,USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;


	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Default.IMC_Default'"));
	if (nullptr != InputMappingContextRef.Object) {
		DefaultMappingContext = InputMappingContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Action/IA_Move.IA_Move'"));
	if (nullptr != InputActionMoveRef.Object) {
		MoveAction = InputActionMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Action/IA_Jump.IA_Jump'"));
	if (nullptr != InputActionJumpRef.Object) {
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Action/IA_Look.IA_Look'"));
	if (nullptr != InputActionLookRef.Object) {
		LookAction = InputActionLookRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionZoomRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/Action/IA_Zoom.IA_Zoom'"));
	if (nullptr != InputActionZoomRef.Object) {
		ZoomAction = InputActionZoomRef.Object;
	}
}

void AALCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
		//Subsystem->RemoveMappingContext(DefaultMappingContext);
	}




}

void AALCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AALCharacterPlayer::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AALCharacterPlayer::Look);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AALCharacterPlayer::Look);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AALCharacterPlayer::Look);
	EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &AALCharacterPlayer::Zoom);

}

void AALCharacterPlayer::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);



}

void AALCharacterPlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);


}

void AALCharacterPlayer::Zoom(const FInputActionValue& Value)
{
	float WheelValue = Value.Get<float>(); // 마우스 휠 축 값 (위=+, 아래=-)

	CameraBoom->TargetArmLength = FMath::Clamp(
		CameraBoom->TargetArmLength - WheelValue * 20.f, // 20은 줌 속도 계수
		100.f, 1000.f // 최소~최대 길이
	);
}




