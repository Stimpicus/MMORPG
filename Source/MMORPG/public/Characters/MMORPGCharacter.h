// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "MMORPGCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class MMORPG_API AMMORPGCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AMMORPGCharacter();

protected:
	virtual void BeginPlay() override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for jump input */
	void Jump() override;

	/** Called for crouch input */
	void ToggleCrouch();

	/** Called for camera zoom input */
	void Zoom(const FInputActionValue& Value);

	/** Called for RMB alignment */
	void AlignCameraStart();
	void AlignCameraStop();

	/** Camera Components */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Input Actions */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ZoomAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* AlignCameraAction;

	/** Camera Settings */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float MinCameraDistance = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float MaxCameraDistance = 800.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float ZoomSpeed = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float FirstPersonThreshold = 120.0f;

private:
	/** Current target arm length for smooth interpolation */
	float CurrentTargetArmLength = 400.0f;

	/** Whether RMB is held for camera alignment */
	bool bIsAlignCameraActive = false;

	/** Updates camera perspective based on zoom level */
	void UpdateCameraPerspective();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Returns CameraBoom subobject **/
	FORCEINLINE USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};
