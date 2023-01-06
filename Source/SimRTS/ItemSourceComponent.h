// Copyright (c) 2022 Snorrisoft ehf
#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemSourceComponent.generated.h"


UCLASS(Blueprintable, ClassGroup=(Items), meta=(BlueprintSpawnableComponent))
class SIMRTS_API UItemSourceComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxQuantity;

	UFUNCTION(BlueprintCallable)
	bool Take(FName InType, int32 InQuantity);
	
	UFUNCTION(BlueprintCallable)
	bool CanPut(FName InType, int32 InQuantity);
	
	UFUNCTION(BlueprintCallable)
	bool Put(FName InType, int32 InQuantity);
	
	UFUNCTION(BluePrintCallable)
	bool IsQuantityAvailable(FName InType, int32 InQuantity);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetQuantity();

	UFUNCTION(BlueprintCallable)
	void SetQuantity(int32 Value);

protected:
	UPROPERTY(BlueprintGetter=GetQuantity, BlueprintSetter=SetQuantity);
	int32 Quantity;


};
