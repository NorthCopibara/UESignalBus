#include "SignalBusComponent.h"

void USignalBusComponent::Bind(UObject* Context, FName const FuncName, const UScriptStruct* SignalType)
{
	if (!Context || !SignalType)
	{
		//TODO: error
		return;
	}

	FSignal Signal;
	if (FString const StructName = SignalType->GetName(); Signals.Contains(StructName))
	{
		for (auto SignalContext : Signals[StructName].SignalsContexts)
		{
			if (SignalContext.Key == FuncName && Context == SignalContext.Value.Context.Get())
			{
				UE_LOG(LogSignalBus, Warning, TEXT("Function %s was binding"), *FuncName.ToString())
				return;
			}
		}

		Signal = Signals[StructName];
	}

	FSignalContext SignalContext{
		Context,
		Context->FindFunctionChecked(FuncName)
	};
	Signal.SignalsContexts.Add(FuncName, MoveTemp(SignalContext));

	Signals.Emplace(SignalType->GetName(), MoveTemp(Signal));
}

void USignalBusComponent::Send(const UScriptStruct* SignalStruct, void* SignalData)
{
	if (!SignalStruct || !SignalData)
	{
		//TODO: error
		return;
	}

	const FString SignalName = SignalStruct->GetName();
	if (!Signals.Contains(SignalName))
	{
		UE_LOG(LogSignalBus, Warning, TEXT("Bindings %s not founded"), *SignalName)
		return;
	}

	TArray<FName> RemovedFunctionsNames;
	for (auto const& SignalContext : Signals[SignalName].SignalsContexts)
	{
		UObject* Context = SignalContext.Value.Context.Get();
		UFunction* Func = SignalContext.Value.Func.Get();
		if (!IsValid(Context) || !Func)
		{
			RemovedFunctionsNames.Add(SignalContext.Key);
			continue;
		}
		Context->ProcessEvent(Func, SignalData);
	}

	for (FName const& RemovedFunctionName : RemovedFunctionsNames)
	{
		RemoveFunction(SignalName, RemovedFunctionName);
	}
}

void USignalBusComponent::Remove(const FName FuncName, const UScriptStruct* SignalType)
{
	RemoveFunction(SignalType->GetName(), FuncName);
}

void USignalBusComponent::Clear()
{
	Signals.Reset();
}

void USignalBusComponent::RemoveFunction(const FString& SignalName, const FName FuncName)
{
	if (!Signals.Contains(SignalName) || !Signals[SignalName].SignalsContexts.Contains(FuncName)) return;
	Signals[SignalName].SignalsContexts.Remove(FuncName);
	if (Signals[SignalName].SignalsContexts.Num() == 0)
		RemoveSignal(SignalName);
}

void USignalBusComponent::RemoveSignal(const FString& SignalName)
{
	if (Signals.Contains(SignalName))
		Signals.Remove(SignalName);
}
