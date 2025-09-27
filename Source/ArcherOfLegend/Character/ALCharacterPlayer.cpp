// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ALCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/ArcharEquipmentData.h"
#include "Animation/ALAnimInstance.h"
#include "GameFramework/Actor.h"
#include "Character/Allow.h"


AALCharacterPlayer::AALCharacterPlayer()
{
	
	/*Load Character*/
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> PlayerMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ElfSelena/BaseMesh/SK_ElfSelena.SK_ElfSelena'"));
	if (PlayerMeshRef.Object) {
		GetMesh()->SetSkeletalMesh(PlayerMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> PlayerAnimInstanceRef(TEXT("/Game/ArcherOfLegend/Animation/ABP_Archer.ABP_Archer_C"));
	if (PlayerAnimInstanceRef.Class) {
		GetMesh()->SetAnimClass(PlayerAnimInstanceRef.Class);
	}


	/*Load Character Weapon Offset*/
	static ConstructorHelpers::FObjectFinder<UArcharEquipmentData> WeaponNoneRef(TEXT("/Script/ArcherOfLegend.ArcharEquipmentData'/Game/ArcherOfLegend/Character/ArcherNoneOffset.ArcherNoneOffset'"));
	if (WeaponNoneRef.Object) {
		WeaponOffsetManager.Add(EWeaponState::None, WeaponNoneRef.Object);
	}
	static ConstructorHelpers::FObjectFinder<UArcharEquipmentData> WeaponHandRef(TEXT("/Script/ArcherOfLegend.ArcharEquipmentData'/Game/ArcherOfLegend/Character/ArcherHandOffset.ArcherHandOffset'"));
	if (WeaponHandRef.Object) {
		WeaponOffsetManager.Add(EWeaponState::Equipped, WeaponHandRef.Object);
	}
	static ConstructorHelpers::FObjectFinder<UArcharEquipmentData> WeaponBackRef(TEXT("/Script/ArcherOfLegend.ArcharEquipmentData'/Game/ArcherOfLegend/Character/ArcherBackOffset.ArcherBackOffset'"));
	if (WeaponBackRef.Object) {
		WeaponOffsetManager.Add(EWeaponState::Sheathed, WeaponBackRef.Object);
	}

	/*Load Character Weapon*/
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> WeaponRef(TEXT("/Script/Engine.SkeletalMesh'/Game/ElfSelena/BaseMesh/Separate/SK_bow.SK_bow'"));
	if (WeaponRef.Object) {
		WeaponState = EWeaponState::Sheathed;
		Weapon->SetSkeletalMesh(WeaponRef.Object);

		static ConstructorHelpers::FClassFinder<UAnimInstance> WeaponAnimInstanceRef(TEXT("/Game/ArcherOfLegend/Animation/ABP_Bow.ABP_Bow_C"));
		if (WeaponAnimInstanceRef.Class) {
			Weapon->SetAnimClass(WeaponAnimInstanceRef.Class);
		}

	}

	
	//Camera Setting
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 700.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom,USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	//Input Setting
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/ArcherOfLegend/Input/IMC_Default.IMC_Default'"));
	if (nullptr != InputMappingContextRef.Object) {
		DefaultMappingContext = InputMappingContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArcherOfLegend/Input/Action/IA_Move.IA_Move'"));
	if (nullptr != InputActionMoveRef.Object) {
		MoveAction = InputActionMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArcherOfLegend/Input/Action/IA_Jump.IA_Jump'"));
	if (nullptr != InputActionJumpRef.Object) {
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArcherOfLegend/Input/Action/IA_Look.IA_Look'"));
	if (nullptr != InputActionLookRef.Object) {
		LookAction = InputActionLookRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionZoomRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArcherOfLegend/Input/Action/IA_Zoom.IA_Zoom'"));
	if (nullptr != InputActionZoomRef.Object) {
		ZoomAction = InputActionZoomRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> MouseAttackActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArcherOfLegend/Input/Action/IA_MouseAttack.IA_MouseAttack'"));
	if (nullptr != MouseAttackActionRef.Object) {
		MouseAttackAction = MouseAttackActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionWeaponToggleRef(TEXT("/Script/EnhancedInput.InputAction'/Game/ArcherOfLegend/Input/Action/IA_WeaponToggle.IA_WeaponToggle'"));
	if (nullptr != InputActionWeaponToggleRef.Object) {
		WeaponToggleAction = InputActionWeaponToggleRef.Object;
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

	//SetWeapon
	Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponOffsetManager[WeaponState]->WeaponComponentName);
	Weapon->SetRelativeRotation(WeaponOffsetManager[WeaponState]->WeaponOffset.GetRotation());
	Weapon->SetRelativeLocation(WeaponOffsetManager[WeaponState]->WeaponOffset.GetLocation());


}

void AALCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AALCharacterPlayer::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AALCharacterPlayer::Look);
	EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &AALCharacterPlayer::Zoom);
	EnhancedInputComponent->BindAction(WeaponToggleAction, ETriggerEvent::Triggered, this, &AALCharacterPlayer::WeaponToggle);
	EnhancedInputComponent->BindAction(MouseAttackAction, ETriggerEvent::Triggered, this, &AALCharacterPlayer::MouseAttack);

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

void AALCharacterPlayer::MouseAttack(const FInputActionValue& value)
{

	bool bIsPressed = value.Get<bool>();
	if (bIsPressed){ //Press Key

		if (USkeletalMeshComponent* MeshComp = GetMesh()) {
			if (UALAnimInstance* AnimInstance = Cast<UALAnimInstance>(MeshComp->GetAnimInstance())) {
				if (!busedupper) {//Once Playing

					//PreSetting Play Motage
					busedupper = true;
					AnimInstance->SetUpperBlendWeight(1.0f);
					
					//PlayMotage
					AnimInstance->Montage_Play(WeaponOffsetManager[WeaponState]->AttackMontage);
					FOnMontageEnded EndDelegate;
					EndDelegate.BindUObject(this, &AALCharacterPlayer::EndMouseAttackAnimation);
					AnimInstance->Montage_SetEndDelegate(EndDelegate, WeaponOffsetManager[WeaponState]->AttackMontage);
			
					//Weapon Animation
					if (WeaponState == EWeaponState::Equipped) { 
						UAnimInstance* pWeaponAnimInstance = Weapon->GetAnimInstance();
						pWeaponAnimInstance->Montage_Play(WeaponOffsetManager[WeaponState]->AttackWeaponMontage);
					}

				}
			}
		}
	}
	else { // Release
		
		if (USkeletalMeshComponent* MeshComp = GetMesh()) {
			if (UALAnimInstance* AnimInstance = Cast<UALAnimInstance>(MeshComp->GetAnimInstance())) {

				//Hand has Weapon
				if (WeaponState == EWeaponState::Equipped) {

					if (AnimInstance->GetBowState() == EBowState::EndPullBack){// deteminate attack by Animation Length
						//PlayerAnimation
						AnimInstance->Montage_Resume(WeaponOffsetManager[WeaponState]->AttackMontage);

						//weaponAnimation
						UAnimInstance* pWeaponAnimInstance = Weapon->GetAnimInstance();
						pWeaponAnimInstance->Montage_Resume(WeaponOffsetManager[WeaponState]->AttackWeaponMontage);
						
						//FireAllow
						fireAllow();
					}
					else if(AnimInstance->GetBowState() == EBowState::StartPullBack){
						//PlayerAnimation
						AnimInstance->Montage_SetPosition(WeaponOffsetManager[WeaponState]->AttackMontage,29.f);

						//weaponAnimation
						UAnimInstance* pWeaponAnimInstance = Weapon->GetAnimInstance();
						pWeaponAnimInstance->Montage_SetPosition(WeaponOffsetManager[WeaponState]->AttackWeaponMontage,29.f);


					}
					else {  //Not Range Pull Back Bow
						//Presetting Animation
						AnimInstance->SetBowState(EBowState::None);
						busedupper = false;
						AnimInstance->SetUpperBlendWeight(0.0f);
						AnimInstance->Montage_Stop(0.0f);

						//weaponAnimation
						UAnimInstance* pWeaponAnimInstance = Weapon->GetAnimInstance();
						pWeaponAnimInstance->Montage_Stop(0.0f);
						//Hide Animation 
						FOnMontageEnded EndDelegate;
						AnimInstance->Montage_SetEndDelegate(EndDelegate, WeaponOffsetManager[WeaponState]->AttackWeaponMontage);
					}
	

					
				}
				else if (WeaponState == EWeaponState::Sheathed) {
					//Not Thing
				}


			}


		}
	}

}

void AALCharacterPlayer::EndMouseAttackAnimation(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	if (USkeletalMeshComponent* MeshComp = GetMesh()) {
		if (UALAnimInstance* AnimInstance = Cast<UALAnimInstance>(MeshComp->GetAnimInstance())) {
			
			//End UpperMotageSlot
			AnimInstance->SetBowState(EBowState::None);
			busedupper = false;
			AnimInstance->SetUpperBlendWeight(0.0f);

		}
	}
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

void AALCharacterPlayer::WeaponToggle(const FInputActionValue& Value)
{

	if (USkeletalMeshComponent* MeshComp = GetMesh()) {
		if (UALAnimInstance* AnimInstance = Cast<UALAnimInstance>(MeshComp->GetAnimInstance())) {
			
			if (AnimInstance->GetBowState() == EBowState::None && !busedupper) {//Not Low Bow And Not Used Upper Slot!

					if (WeaponState == EWeaponState::Sheathed) {
						WeaponState = EWeaponState::Equipped;
					}
					else if (WeaponState == EWeaponState::Equipped) {
						WeaponState = EWeaponState::Sheathed;
					}

					SetWeapon(WeaponState);
			}
		}
	}

}


void AALCharacterPlayer::SetWeapon(const EWeaponState& state)
{

	if (USkeletalMeshComponent* MeshComp = GetMesh()) {
		if (UALAnimInstance* AnimInstance = Cast<UALAnimInstance>(MeshComp->GetAnimInstance())) {

			if (!AnimInstance->IsAnyMontagePlaying()) {//Once Playing
				
					AnimInstance->Montage_Play(WeaponOffsetManager[state]->Montage);

					FOnMontageEnded EndDelegate;
					EndDelegate.BindUObject(this, &AALCharacterPlayer::EndWeaponAnimation);
					AnimInstance->Montage_SetEndDelegate(EndDelegate, WeaponOffsetManager[state]->Montage);
					AnimInstance->SetUpperBlendWeight(1.0f);
					busedupper = true; 
				
			}
		}
	}
}

void AALCharacterPlayer::EndWeaponAnimation(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	if (USkeletalMeshComponent* MeshComp = GetMesh()){

		if (UALAnimInstance* AnimInstance = Cast<UALAnimInstance>(MeshComp->GetAnimInstance())){

			busedupper = false;
			AnimInstance->SetUpperBlendWeight(0.0f);

			Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponOffsetManager[WeaponState]->WeaponComponentName);
			Weapon->SetRelativeRotation(WeaponOffsetManager[WeaponState]->WeaponOffset.GetRotation());
			Weapon->SetRelativeLocation(WeaponOffsetManager[WeaponState]->WeaponOffset.GetLocation());

			AnimInstance->SetWeaponState(WeaponState == EWeaponState::Equipped);
		}
	}
	
}

void AALCharacterPlayer::SwapWeaponHand(int step)
{
	if (step == 1) {
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponOffsetManager[WeaponState]->FirstWeaponComponentName);
		Weapon->SetRelativeRotation(WeaponOffsetManager[WeaponState]->FirstWeaponOffset.GetRotation());
		Weapon->SetRelativeLocation(WeaponOffsetManager[WeaponState]->FirstWeaponOffset.GetLocation());
	}
	else if(step == 2){
		Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponOffsetManager[WeaponState]->SecondWeaponComponentName);
		Weapon->SetRelativeRotation(WeaponOffsetManager[WeaponState]->SecondWeaponOffset.GetRotation());
		Weapon->SetRelativeLocation(WeaponOffsetManager[WeaponState]->SecondWeaponOffset.GetLocation());
	}
}

void AALCharacterPlayer::PauseBowLine()
{
	if (WeaponState == EWeaponState::Equipped) { // If Hand Has Weapon 
		UAnimInstance* pWeaponAnimInstance = Weapon->GetAnimInstance();
		pWeaponAnimInstance->Montage_Pause();
	}
}

void AALCharacterPlayer::fireAllow()
{

	const FVector SpawnLocation = GetActorLocation();

	GetWorld()->SpawnActor(AAllow::StaticClass(), &SpawnLocation, &FRotator::ZeroRotator);


}

