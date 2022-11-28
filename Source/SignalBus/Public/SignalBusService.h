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
	template <typename T, typename UserClass>
	void Bind(UserClass* Context, FName FuncName)
	{
		FSignal Signal;

		if (Signals.Contains(T::StaticStruct()->GetName()))
		{
			for (auto SignalContext : Signals[T::StaticStruct()->GetName()].SignalsContexts)
			{
				if(SignalContext.FuncName == FuncName && Context == SignalContext.Context.Get())
				{
					UE_LOG(LogSignalBus, Warning, TEXT("Function %s was binding"), *FuncName.ToString())
					return;
				}
			}
			
			Signal = Signals[T::StaticStruct()->GetName()];
		}

		FSignalContext SignalContext;
		SignalContext.Context = Cast<UObject>(Context);
		SignalContext.Func = SignalContext.Context->FindFunctionChecked(FuncName);
		SignalContext.FuncName = FuncName;
		Signal.SignalsContexts.Add(SignalContext);

		Signals.Emplace(T::StaticStruct()->GetName(), Signal);
	}

	template <typename T>
	void Send(T SignalData)
	{
		if (!Signals.Contains(T::StaticStruct()->GetName()))
		{
			UE_LOG(LogSignalBus, Warning, TEXT("Bindings not found"))
			return;
		}

		void* OutData = static_cast<void*>(&SignalData);

		const auto Signal = Signals[T::StaticStruct()->GetName()];
		for (auto SignalContext : Signal.SignalsContexts)
		{
			SignalContext.Context->ProcessEvent(SignalContext.Func.Get(), OutData);
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
