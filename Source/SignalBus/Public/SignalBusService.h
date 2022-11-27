// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SignalBusService.generated.h"

USTRUCT(BlueprintType)
struct FSignal
{
	GENERATED_BODY()

	TWeakObjectPtr<UObject> Context;
	UFunction* Func;
};


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAction, FSignal, Signal);


UCLASS()
class SIGNALBUS_API USignalBusService : public UObject
{
	GENERATED_BODY()
	
public:
	template <typename T, typename UserClass>
	void Bind(UserClass* Context, FName FuncName)
	{
		if(Signals.Contains(T::StaticStruct()->GetName()))
		{
			//TODO
			UE_LOG(LogTemp, Error, TEXT("Govno"))
			return;
		}
		
		FSignal Signal;
		Signal.Context = Cast<UObject>(Context);
		Signal.Func = Signal.Context->FindFunction(FuncName);

		UE_LOG(LogTemp, Warning, TEXT("Test_func: %s"), *FuncName.ToString())
		UE_LOG(LogTemp, Warning, TEXT("Test_bind: %s"), *T::StaticStruct()->GetName())
		
		Signals.Emplace(T::StaticStruct()->GetName(), Signal);
	}

	template <typename T>
	void Send(T SignalData)
	{
		if(Signals.Contains(T::StaticStruct()->GetName()))
		{
			void* OutData = static_cast<void*>(&SignalData);
			
			const auto Signal = Signals[T::StaticStruct()->GetName()];
			Signal.Context->ProcessEvent(Signal.Func, OutData);
		}
	}

	void Clear()
	{
		Signals.Empty();
	}

private:
	UPROPERTY()
	TMap<FString, FSignal> Signals;
};
