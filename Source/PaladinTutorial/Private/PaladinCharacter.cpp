// Fill out your copyright notice in the Description page of Project Settings.


#include "PaladinCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

//
// Sets default values
APaladinCharacter::APaladinCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create camera boom
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->TargetArmLength = 400;
	SpringArmComponent->bUsePawnControlRotation = true;

	// Create follow camera
	FollowCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
	FollowCameraComponent->bUsePawnControlRotation = false;

	// Jump settings
	GetCharacterMovement()->JumpZVelocity = 300.0f;
	GetCharacterMovement()->AirControl = 0.1f;
}

// Called when the game starts or when spawned
void APaladinCharacter::BeginPlay()
{
	Super::BeginPlay();

	// add input mapping content
	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		// Get local player subsystem
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// Add input context
			Subsystem->AddMappingContext(InputMapping, 0);
		}
	}
}

void APaladinCharacter::Move(const FInputActionValue& InputValue)
{
	const FVector2d InputVector = InputValue.Get<FVector2D>();

	if (IsValid(Controller))
	{
		// Get forward direction
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		// Rotation Matrix
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);

		// Add movement input
		AddMovementInput(ForwardDirection, InputVector.Y);
		AddMovementInput(RightDirection, InputVector.X);
	}
}

void APaladinCharacter::Look(const FInputActionValue& InputValue)
{
	const FVector2D InputVector = InputValue.Get<FVector2D>();

	if (IsValid(Controller))
	{
		AddControllerYawInput(InputVector.X);
		AddControllerPitchInput(-InputVector.Y);
	}
}

inline void APaladinCharacter::Jump()
{
	// Call parent class jump function
	Super::Jump();

	if (GetCharacterMovement()->IsMovingOnGround())
	{
		// Get current forward velocity
		FVector ForwardVelocity = GetVelocity();
		ForwardVelocity.Z = 0.0f;

		// Define jump vertical velocity
		float JumpVerticalVelocity = GetCharacterMovement()->JumpZVelocity;

		// Combine current forward velocity with jump vertical velocity
		// 現在の前進速度とジャンプの垂直速度を組み合わせる
		FVector jumpVelocity = ForwardVelocity + FVector(0.0f, 0.0f, JumpVerticalVelocity);

		// Launch character with the combined velocity
		// 組み合わせた速度でキャラクターを起動する
		LaunchCharacter(jumpVelocity, true, true);
	}
}

// Called every frame
void APaladinCharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APaladinCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Movement actions
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APaladinCharacter::Move);
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &APaladinCharacter::Look);
		Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &APaladinCharacter::Jump);
	}
}

