// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

void AMyPlayerController::SetupInputs()
{
	if (PossessedPawn)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			FModifyContextOptions Options;
			Options.bForceImmediately = 1;
			Subsystem->AddMappingContext(DefaultInputMappingContext, 1, Options);
		}
	}
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->ClearActionEventBindings();
		EnhancedInputComponent->ClearActionValueBindings();
		EnhancedInputComponent->ClearDebugKeyBindings();

		BindActions(DefaultInputMappingContext);
	}
}

void AMyPlayerController::BindActions(UInputMappingContext* Context)
{
	if (Context)
	{
		const TArray<FEnhancedActionKeyMapping>& Mappings = Context->GetMappings();
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
		{
			// There may be more than one keymapping assigned to one action. So, first filter duplicate action entries to prevent multiple delegate bindings
			TSet<const UInputAction*> UniqueActions;
			for (const FEnhancedActionKeyMapping& Keymapping : Mappings)
			{
				UniqueActions.Add(Keymapping.Action);
			}
			for (const UInputAction* UniqueAction : UniqueActions)
			{
				EnhancedInputComponent->BindAction(UniqueAction, ETriggerEvent::Triggered, Cast<UObject>(this), UniqueAction->GetFName());
			}
		}
	}
}

void AMyPlayerController::OnPossess(APawn* InPawn)
{

	Super::OnPossess(InPawn);
	PossessedPawn = Cast<AMyPawn>(InPawn);
	SetupInputs();
}

void AMyPlayerController::ForwardMovementAction(const FInputActionValue& Value)
{
	if (PossessedPawn)
	{
		PossessedPawn->ForwardMovementAction(Value.GetMagnitude());
	}
}

void AMyPlayerController::RightMovementAction(const FInputActionValue& Value)
{
	if (PossessedPawn)
	{
		PossessedPawn->RightMovementAction(Value.GetMagnitude());
	}
}

void AMyPlayerController::CameraPitchAction(const FInputActionValue& Value)
{
	if (PossessedPawn)
	{
		PossessedPawn->CameraPitchAction(Value.GetMagnitude());
	}
}

void AMyPlayerController::CameraRotateAction(const FInputActionValue& Value)
{
	if (PossessedPawn)
	{
		PossessedPawn->CameraRotateAction(Value.GetMagnitude());
	}
}
