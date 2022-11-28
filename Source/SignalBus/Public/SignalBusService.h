// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SignalBusService.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogSignalBus, All, All);

USTRUCT(BlueprintType)
struct FSignalContext
{
	GENERATED_BODY()

	FName FuncName;
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
	template <typename T>
	void Bind(UObject* Context, FName FuncName)
	{
		Bind(Context, FuncName, T::StaticStruct());
	}

	void Bind(UObject* Context, FName FuncName, const UScriptStruct* SignalType)
	{
		FSignal Signal;

		if (Signals.Contains(SignalType->GetName()))
		{
			for (auto SignalContext : Signals[SignalType->GetName()].SignalsContexts)
			{
				if (SignalContext.FuncName == FuncName && Context == SignalContext.Context.Get())
				{
					UE_LOG(LogSignalBus, Warning, TEXT("Function %s was binding"), *FuncName.ToString())
					return;
				}
			}

			Signal = Signals[SignalType->GetName()];
		}

		FSignalContext SignalContext;
		SignalContext.Context = Cast<UObject>(Context);
		SignalContext.Func = SignalContext.Context->FindFunctionChecked(FuncName);
		SignalContext.FuncName = FuncName;
		Signal.SignalsContexts.Add(SignalContext);

		Signals.Emplace(SignalType->GetName(), Signal);
	}

	template <typename T>
	void Send(T SignalData)
	{
		Send(T::StaticStruct(), static_cast<void*>(&SignalData));
	}

	void Send(const UScriptStruct* SignalStruct, void* SignalData)
	{
		if (!Signals.Contains(SignalStruct->GetName()))
		{
			UE_LOG(LogSignalBus, Warning, TEXT("Bindings not found"))
			return;
		}
		
		const auto Signal = Signals[SignalStruct->GetName()];
		for (auto SignalContext : Signal.SignalsContexts)
		{
			SignalContext.Context->ProcessEvent(SignalContext.Func.Get(), SignalData);
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
