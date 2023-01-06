// Copyright (c) 2022 Snorrisoft ehf

#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "ItemSourceComponent.h"
#include "TransformerComponent.generated.h"

UENUM(BlueprintType)
enum class ETransformerState : uint8
{
	Initializing,
	WaitingForInputs,
	Transforming,
	OutputFull
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStateChangedDelegate, ETransformerState, OldState);

/**
 * Transforms items from one type to another according to a recipe. Assumes the presence
 * of an inventory component to hold inputs and outputs.
 */
UCLASS(Blueprintable, ClassGroup=(Items), meta=(BlueprintSpawnableComponent))
class SIMRTS_API UTransformerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UTransformerComponent();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FInventoryItem> Inputs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FInventoryItem> Outputs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TransformationTime = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ETransformerState State = ETransformerState::Initializing;

	UPROPERTY(BlueprintAssignable)
	FStateChangedDelegate OnInitializing;
	
	UPROPERTY(BlueprintAssignable)
	FStateChangedDelegate OnWaitingForInputs;
	
	UPROPERTY(BlueprintAssignable)
	FStateChangedDelegate OnTransformingStarted;
	
	UPROPERTY(BlueprintAssignable)
	FStateChangedDelegate OnItemProduced;
	
	UPROPERTY(BlueprintAssignable)
	FStateChangedDelegate OnOutputsFull;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentTransformationTime = 0.0f;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

protected:
	void StartTransforming();
	void StartWaitingForInputs();
	void WaitForInputs();
	UItemSourceComponent* FindOutputReceiver(FInventoryItem Item);
	bool CheckOutputsFull();
	void Transform(float DeltaTime);
	void OutputsFull();
};
