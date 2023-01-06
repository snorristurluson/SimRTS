// Copyright (c) 2022 Snorrisoft ehf

#include "ItemSourceComponent.h"

bool UItemSourceComponent::Take(FName InType, int32 InQuantity)
{
	if (InType != Type)
	{
		return false;
	}
	if (Quantity < InQuantity)
	{
		return false;
	}

	Quantity -= InQuantity;
	return true;
}

bool UItemSourceComponent::CanPut(FName InType, int32 InQuantity)
{
	if (InType != Type)
	{
		return false;
	}

	if (MaxQuantity == 0 || Quantity + InQuantity <= MaxQuantity)
	{
		return true;
	}

	return false;
}

bool UItemSourceComponent::Put(FName InType, int32 InQuantity)
{
	if (InType != Type)
	{
		return false;
	}

	Quantity += InQuantity;
	if (MaxQuantity > 0 && Quantity > MaxQuantity)
	{
		Quantity = MaxQuantity;
	}

	return true;
}

bool UItemSourceComponent::IsQuantityAvailable(FName InType, int32 InQuantity)
{
	return Type == InType && Quantity >= InQuantity;
}

int32 UItemSourceComponent::GetQuantity()
{
	return Quantity;
}

void UItemSourceComponent::SetQuantity(int32 Value)
{
	Quantity = Value;
}
