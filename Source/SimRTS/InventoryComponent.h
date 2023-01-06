// Copyright (c) 2022 Snorrisoft ehf

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

USTRUCT(BlueprintType)
struct SIMRTS_API FInventoryItem {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quantity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxQuantity = 1000;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInventoryChangedDelegate);

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class SIMRTS_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FInventoryItem> Slots;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool CanAdd(FName Name, int32 Quantity);

	UFUNCTION(BlueprintCallable)
	int32 Add(FName Name, int32 Quantity);

	UFUNCTION(BlueprintCallable)
	bool Take(FName Type, int32 Quantity);
	
	UFUNCTION(BluePrintCallable, BlueprintPure)
	bool IsQuantityAvailable(FName Type, int32 Quantity);
	
	UFUNCTION(BluePrintCallable, BlueprintPure)
	int32 GetQuantity(FName Type);
	
	UFUNCTION(BluePrintCallable, BlueprintPure)
	bool AreAllAvailable(const TArray<FInventoryItem>& Items);
	
	UFUNCTION(BluePrintCallable)
	bool TakeAll(const TArray<FInventoryItem>& Items);
	
	UFUNCTION(BluePrintCallable, BlueprintPure)
	bool CanAddAll(const TArray<FInventoryItem>& Items);
	
	UFUNCTION(BluePrintCallable)
	bool AddAll(const TArray<FInventoryItem>& Items);
	
	UPROPERTY(BlueprintAssignable)
	FInventoryChangedDelegate OnInventoryChanged;
	
protected:
	bool AddToFirstAvailableSlotHelper(FName NameToCheck, FName NameToAdd, int32 Quantity);

};
