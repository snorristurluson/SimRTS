// Copyright (c) 2022 Snorrisoft ehf


#include "ItemSinkComponent.h"


void UItemSinkComponent::AddRequestForType(FName InType, int32 InQuantity)
{
	bool SlotFound = false;
	for (int32 SlotIndex = 0; SlotIndex < Requests.Num(); ++SlotIndex)
	{
		FInventoryItem& Slot = Requests[SlotIndex];
		if (Slot.Type == InType)
		{
			Slot.Quantity += InQuantity;
			SlotFound = true;
			break;
		}
	}
	if (!SlotFound)
	{
		FInventoryItem& NewSlot = Requests.AddDefaulted_GetRef();
		NewSlot.Type = InType;
		NewSlot.Quantity = InQuantity;
	}
}

int32 UItemSinkComponent::GetItemsNeededForType(FName InType)
{
	for (int32 SlotIndex = 0; SlotIndex < Requests.Num(); ++SlotIndex)
	{
		FInventoryItem& Slot = Requests[SlotIndex];
		if (Slot.Type == InType)
		{
			return Slot.Quantity;
		}
	}

	return 0;
}

FName UItemSinkComponent::GetItemTypeNeeded()
{
	for (auto Request : Requests)
	{
		if (Request.Quantity > 0)
		{
			return Request.Type;
		}
	}

	return FName();
}

bool UItemSinkComponent::Put(FName InType, int32 InQuantity)
{
	for (int32 SlotIndex = 0; SlotIndex < Requests.Num(); ++SlotIndex)
	{
		FInventoryItem& Slot = Requests[SlotIndex];
		if (Slot.Type == InType)
		{
			Slot.Quantity -= InQuantity;
			if (Slot.Quantity <= 0)
			{
				Requests.RemoveAt(SlotIndex);
			}
			OnItemAdded.Broadcast(InType, InQuantity);
			return true;
		}
	}
	return false;
}
