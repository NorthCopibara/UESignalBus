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

	TWeakObjectPtr<UObject> Context;
	TWeakObjectPtr<UFunction> Func;
};

USTRUCT(BlueprintType)
struct FSignal
{
	GENERATED_BODY()

	TMap<FName, FSignalContext> SignalsContexts;
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
				if (SignalContext.Key == FuncName && Context == SignalContext.Value.Context.Get())
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
		Signal.SignalsContexts.Add(FuncName, SignalContext);

		Signals.Emplace(SignalType->GetName(), Signal);
	}

	template <typename T>
	void Send(T SignalData)
	{
		Send(T::StaticStruct(), static_cast<void*>(&SignalData));
	}

	void Send(const UScriptStruct* SignalStruct, void* SignalData)
	{
		const auto SignalName = SignalStruct->GetName();
		if (!Signals.Contains(SignalName))
		{
			UE_LOG(LogSignalBus, Warning, TEXT("Bindings %s not founded"), *SignalName)
			return;
		}

		TArray<FName> RemovedFunctionsNames;

		const auto Signal = Signals[SignalName];
		for (auto SignalContext : Signal.SignalsContexts)
		{
			const auto Context = SignalContext.Value.Context.Get();
			const auto Func = SignalContext.Value.Func.Get();
			if (!Context || !Func)
			{
				RemovedFunctionsNames.Add(SignalContext.Key);
				continue;
			}
			Context->ProcessEvent(Func, SignalData);
		}

		for (const auto RemovedFunctionName : RemovedFunctionsNames)
		{
			RemoveFunction(SignalName, RemovedFunctionName);
		}
	}

	template <typename T>
	void Remove(const FName FuncName)
	{
		Remove(FuncName, T::StaticStruct());
	}

	void Remove(const FName FuncName, const UScriptStruct* SignalType)
	{
		RemoveFunction(SignalType->GetName(), FuncName);
	}

	void Clear()
	{
		Signals.Empty();
	}

private:
	void RemoveFunction(FString SignalName, FName FuncName)
	{
		if (Signals.Contains(SignalName) && Signals[SignalName].SignalsContexts.Contains(FuncName))
		{
			Signals[SignalName].SignalsContexts.Remove(FuncName);
			if (Signals[SignalName].SignalsContexts.Num() == 0)
			{
				RemoveSignal(SignalName);
			}
		}
	}

	void RemoveSignal(FString SignalName)
	{
		if (Signals.Contains(SignalName))
		{
			Signals.Remove(SignalName);
		}
	}

private:
	UPROPERTY()
	TMap<FString, FSignal> Signals;
};
