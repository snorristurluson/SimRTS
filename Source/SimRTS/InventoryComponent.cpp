// Copyright (c) 2022 Snorrisoft ehf


#include "InventoryComponent.h"

bool UInventoryComponent::CanAdd(FName Name, int32 Quantity)
{
	// Look for a slot with the given type
	for (int32 SlotIndex = 0; SlotIndex < Slots.Num(); ++SlotIndex)
	{
		FInventoryItem& Slot = Slots[SlotIndex];
		if (Slot.Type == Name && Slot.Quantity + Quantity <= Slot.MaxQuantity)
		{
			return true;
		}
	}

	// Look for an empty slot
	for (int32 SlotIndex = 0; SlotIndex < Slots.Num(); ++SlotIndex)
	{
		FInventoryItem& Slot = Slots[SlotIndex];
		if (Slot.Type.IsNone())
		{
			return true;
		}
	}

	return false;
}

int32 UInventoryComponent::Add(FName Name, int32 Quantity)
{
	int32 FoundIndex = -1;
	
	// Look for a slot with the given type
	for (int32 SlotIndex = 0; SlotIndex < Slots.Num(); ++SlotIndex)
	{
		FInventoryItem& Slot = Slots[SlotIndex];
		if (Slot.Type == Name)
		{
			FoundIndex = SlotIndex;
			break;
		}
	}
	
	if (FoundIndex < 0)
	{
		// Look for an empty slot
		for (int32 SlotIndex = 0; SlotIndex < Slots.Num(); ++SlotIndex)
		{
			FInventoryItem& Slot = Slots[SlotIndex];
			if (Slot.Type.IsNone())
			{
				FoundIndex = SlotIndex;
				break;
			}
		}
	}

	if (FoundIndex >= 0)
	{
		FInventoryItem& Slot = Slots[FoundIndex];
		if (Slot.Quantity + Quantity <= Slot.MaxQuantity)
		{
			Slot.Type = Name;
			Slot.Quantity += Quantity;
			OnInventoryChanged.Broadcast();
			return true;
		}
	}

	return false;
}

bool UInventoryComponent::Take(FName Type, int32 Quantity)
{
	if (!IsQuantityAvailable(Type, Quantity))
	{
		return false;
	}
	int32 Missing = Quantity;
	
	for (int32 SlotIndex = 0; SlotIndex < Slots.Num(); ++SlotIndex)
	{
		FInventoryItem& Slot = Slots[SlotIndex];
		if (Slot.Type == Type)
		{
			int32 QuantityTaken = Missing;
			if (QuantityTaken > Slot.Quantity)
			{
				QuantityTaken = Slot.Quantity;
			}
			Slot.Quantity -= QuantityTaken;
			Missing -= QuantityTaken;
			if (Missing <= 0)
			{
				break;
			}
		}
	}

	OnInventoryChanged.Broadcast();
	return true;
}

bool UInventoryComponent::IsQuantityAvailable(FName Type, int32 Quantity)
{
	int32 Missing = Quantity;
	
	for (int32 SlotIndex = 0; SlotIndex < Slots.Num(); ++SlotIndex)
	{
		FInventoryItem& Slot = Slots[SlotIndex];
		if (Slot.Type == Type)
		{
			Missing -= Slot.Quantity;
			if (Missing <= 0)
			{
				break;
			}
		}
	}

	return Missing <= 0;
}

int32 UInventoryComponent::GetQuantity(FName Type)
{
	int Quantity = 0;
	for (int32 SlotIndex = 0; SlotIndex < Slots.Num(); ++SlotIndex)
	{
		FInventoryItem& Slot = Slots[SlotIndex];
		if (Slot.Type == Type)
		{
			Quantity += Slot.Quantity;
		}
	}
	return Quantity;
}

bool UInventoryComponent::AreAllAvailable(const TArray<FInventoryItem>& Items)
{
	for (auto Item : Items)
	{
		if (!IsQuantityAvailable(Item.Type, Item.Quantity))
		{
			return false;
		}
	}
	return true;
}

bool UInventoryComponent::TakeAll(const TArray<FInventoryItem>& Items)
{
	for (auto Item : Items)
	{
		if (!Take(Item.Type, Item.Quantity))
		{
			return false;
		}
	}
	return true;
}

bool UInventoryComponent::CanAddAll(const TArray<FInventoryItem>& Items)
{
	for (auto Item : Items)
	{
		if (!CanAdd(Item.Type, Item.Quantity))
		{
			return false;
		}
	}
	return true;
}

bool UInventoryComponent::AddAll(const TArray<FInventoryItem>& Items)
{
	for (auto Item : Items)
	{
		if (!Add(Item.Type, Item.Quantity))
		{
			return false;
		}
	}
	return true;
}
