// Copyright (c) 2022 Snorrisoft ehf

#pragma once

#include "CoreMinimal.h"
#include "InventoryComponent.h"
#include "Components/ActorComponent.h"
#include "ItemSinkComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FItemAddedDelegate, FName, Type, int32, Quantity);

UCLASS(Blueprintable, ClassGroup=(Items), meta=(BlueprintSpawnableComponent))
class SIMRTS_API UItemSinkComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void AddRequestForType(FName InType, int32 InQuantity);
	
	UFUNCTION(BlueprintCallable)
	int32 GetItemsNeededForType(FName InType);
	
	UFUNCTION(BlueprintCallable)
	FName GetItemTypeNeeded();
	
	UFUNCTION(BlueprintCallable)
	bool Put(FName InType, int32 InQuantity);

	UPROPERTY(BlueprintAssignable)
	FItemAddedDelegate OnItemAdded;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FInventoryItem> Requests;
};
