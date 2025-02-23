// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PaladinCharacter.generated.h"


// Declaration
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class PALADINTUTORIAL_API APaladinCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APaladinCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Input Actions
	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	UInputMappingContext* InputMapping;
	
	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	UInputAction* JumpAction;

	// Movement
	void Move(const FInputActionValue& InputValue);
	void Look(const FInputActionValue& InputValue);
	void Jump();

private:
	// Spring arm component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComponent;

	// Follow camera component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCameraComponent;

};























