// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MyPawn.h"
#include "InputActionValue.h"
#include "MyPlayerController.generated.h"

class UInputMappingContext;
/**
 * 
 */
UCLASS()
class SIMRTS_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultInputMappingContext = nullptr;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AMyPawn> PossessedPawn = nullptr;
	
protected:
	void SetupInputs();
	virtual void SetupInputComponent() override;
	virtual void BindActions(UInputMappingContext* Context);
	virtual void OnPossess(APawn* InPawn) override;
	
	UFUNCTION()
	void ForwardMovementAction(const FInputActionValue& Value);

	UFUNCTION()
	void RightMovementAction(const FInputActionValue& Value);

	UFUNCTION()
	void CameraPitchAction(const FInputActionValue& Value);

	UFUNCTION()
	void CameraRotateAction(const FInputActionValue& Value);
};
