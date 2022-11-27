// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SignalBusService.generated.h"

USTRUCT(BlueprintType)
struct FSignalContext
{
	GENERATED_BODY()

	TWeakObjectPtr<UObject> Context;
	TWeakObjectPtr<UFunction> Func;
};

USTRUCT(BlueprintType)
struct FSignal
{
	GENERATED_BODY()

	TArray<FSignalContext> SignalsContexts;
};

UCLASS()
class SIGNALBUS_API USignalBusService : public UObject
{
	GENERATED_BODY()
	
public:
	template <typename T, typename UserClass>
	void Bind(UserClass* Context, FName FuncName)
	{
		FSignal Signal;
		
		if(Signals.Contains(T::StaticStruct()->GetName()))
		{
			Signal = Signals[T::StaticStruct()->GetName()];
		}

		FSignalContext SignalContext;
		SignalContext.Context = Cast<UObject>(Context);
		SignalContext.Func = SignalContext.Context->FindFunction(FuncName);
		Signal.SignalsContexts.Add(SignalContext);

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
			for (auto SignalContext : Signal.SignalsContexts)
			{
				SignalContext.Context->ProcessEvent(SignalContext.Func.Get(), OutData);
			}
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
