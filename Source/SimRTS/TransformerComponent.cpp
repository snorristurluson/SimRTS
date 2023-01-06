// Copyright (c) 2022 Snorrisoft ehf


#include "TransformerComponent.h"

#include "ItemSourceComponent.h"

void UTransformerComponent::WaitForInputs()
{
	UInventoryComponent* Inventory = GetOwner()->FindComponentByClass<UInventoryComponent>();
	if (Inventory)
	{
		if (Inventory->AreAllAvailable(Inputs))
		{
			Inventory->TakeAll(Inputs);
			StartTransforming();
		}
	}
}

UItemSourceComponent* UTransformerComponent::FindOutputReceiver(FInventoryItem Item)
{
	UItemSourceComponent* Receiver = nullptr;
	auto OutputComponents = GetOwner()->GetComponentsByTag(UItemSourceComponent::StaticClass(), Item.Type);
	if (OutputComponents.Num() > 0)
	{
		Receiver = Cast<UItemSourceComponent>(OutputComponents[0]);
	}
	return Receiver;
}

bool UTransformerComponent::CheckOutputsFull()
{
	bool AreOutputsFull = false;
	for (auto Item : Outputs)
	{
		if (UItemSourceComponent* Receiver = FindOutputReceiver(Item))
		{
			if (!Receiver->CanPut(Item.Type, Item.Quantity))
			{
				AreOutputsFull = true;
				break;
			}
		}
	}
	return AreOutputsFull;
}

void UTransformerComponent::Transform(float DeltaTime)
{
	CurrentTransformationTime += DeltaTime;
	if (CurrentTransformationTime >= TransformationTime)
	{
		if (bool AreOutputsFull = CheckOutputsFull())
		{
			OnOutputsFull.Broadcast(State);
			State = ETransformerState::OutputFull;
			return;
		}
		for (auto Item : Outputs)
		{
			if (UItemSourceComponent* Receiver = FindOutputReceiver(Item))
			{
				Receiver->Put(Item.Type, Item.Quantity);
			}
		}
		OnItemProduced.Broadcast(State);
		StartWaitingForInputs();
	}
}

void UTransformerComponent::OutputsFull()
{
	bool AreOutputsFull = CheckOutputsFull();
	if (!AreOutputsFull)
	{
		StartTransforming();
	}
}

void UTransformerComponent::StartTransforming()
{
	OnTransformingStarted.Broadcast(State);
	State = ETransformerState::Transforming;
	CurrentTransformationTime = 0.0f;
}

void UTransformerComponent::StartWaitingForInputs()
{
	OnWaitingForInputs.Broadcast(State);
	State = ETransformerState::WaitingForInputs;
}

UTransformerComponent::UTransformerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UTransformerComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (State)
	{
	case ETransformerState::Initializing:
		OnInitializing.Broadcast(ETransformerState::Initializing);
		StartWaitingForInputs();
		break;

	case ETransformerState::WaitingForInputs:
		WaitForInputs();
		break;

	case ETransformerState::Transforming:
		Transform(DeltaTime);
		break;

	case ETransformerState::OutputFull:
		OutputsFull();
		break;
	}
}
