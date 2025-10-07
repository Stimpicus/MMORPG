// Copyright Epic Games, Inc. All Rights Reserved.

#include "Characters/MMORPGCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

AMMORPGCharacter::AMMORPGCharacter()
{
	// Set this character to call Tick() every frame
	PrimaryActorTick.bCanEverTick = true;

	// Don't rotate when the controller rotates. Let the camera handle rotation.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Configure crouch settings
	GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
	GetCharacterMovement()->CrouchedHalfHeight = 60.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 10.0f;
	CameraBoom->bDoCollisionTest = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Initialize current target arm length
	CurrentTargetArmLength = CameraBoom->TargetArmLength;
}

void AMMORPGCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AMMORPGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Smoothly interpolate the camera boom's target arm length
	if (CameraBoom)
	{
		CameraBoom->TargetArmLength = FMath::FInterpTo(
			CameraBoom->TargetArmLength,
			CurrentTargetArmLength,
			DeltaTime,
			10.0f
		);

		UpdateCameraPerspective();
	}
}

void AMMORPGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Moving
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMMORPGCharacter::Move);
		}

		// Looking
		if (LookAction)
		{
			EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMMORPGCharacter::Look);
		}

		// Jumping
		if (JumpAction)
		{
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AMMORPGCharacter::Jump);
			EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		}

		// Crouching
		if (CrouchAction)
		{
			EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AMMORPGCharacter::ToggleCrouch);
		}

		// Zooming
		if (ZoomAction)
		{
			EnhancedInputComponent->BindAction(ZoomAction, ETriggerEvent::Triggered, this, &AMMORPGCharacter::Zoom);
		}

		// Camera Alignment
		if (AlignCameraAction)
		{
			EnhancedInputComponent->BindAction(AlignCameraAction, ETriggerEvent::Started, this, &AMMORPGCharacter::AlignCameraStart);
			EnhancedInputComponent->BindAction(AlignCameraAction, ETriggerEvent::Completed, this, &AMMORPGCharacter::AlignCameraStop);
		}
	}
}

void AMMORPGCharacter::Move(const FInputActionValue& Value)
{
	// Input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// Find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// Get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// Add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AMMORPGCharacter::Look(const FInputActionValue& Value)
{
	// Input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// Add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AMMORPGCharacter::Jump()
{
	Super::Jump();
}

void AMMORPGCharacter::ToggleCrouch()
{
	if (bIsCrouched)
	{
		UnCrouch();
	}
	else
	{
		Crouch();
	}
}

void AMMORPGCharacter::Zoom(const FInputActionValue& Value)
{
	// Input is a single float
	float ZoomValue = Value.Get<float>();

	// Adjust target arm length
	CurrentTargetArmLength = FMath::Clamp(
		CurrentTargetArmLength - (ZoomValue * ZoomSpeed),
		MinCameraDistance,
		MaxCameraDistance
	);
}

void AMMORPGCharacter::AlignCameraStart()
{
	bIsAlignCameraActive = true;
	
	if (Controller != nullptr)
	{
		// Align character rotation to camera direction
		const FRotator ControlRotation = Controller->GetControlRotation();
		const FRotator NewRotation(0, ControlRotation.Yaw, 0);
		SetActorRotation(NewRotation);
	}
}

void AMMORPGCharacter::AlignCameraStop()
{
	bIsAlignCameraActive = false;
}

void AMMORPGCharacter::UpdateCameraPerspective()
{
	if (!CameraBoom || !FollowCamera)
		return;

	// Check if we've zoomed in enough to switch to first-person
	if (CameraBoom->TargetArmLength <= FirstPersonThreshold)
	{
		// First-person mode: attach camera to character's head
		// For a more accurate first-person view, you might want to use a socket on the mesh
		FollowCamera->SetRelativeLocation(FVector(0.0f, 0.0f, 60.0f)); // Approximate eye level
	}
	else
	{
		// Third-person mode: reset camera to default position on spring arm
		FollowCamera->SetRelativeLocation(FVector::ZeroVector);
	}
}
