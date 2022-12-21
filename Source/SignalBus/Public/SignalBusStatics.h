#pragma once

#include "SignalBusComponent.h"
#include "SignalBusService.h"
#include "GameFramework/GameModeBase.h"


class SignalBusStatics
{
public:
	template <typename SignalType>
	static void Bind(UObject* WorldContext, FName FuncName)
	{
		const auto SignalBus = GetSignalBus(WorldContext);
		SignalBus->Bind<SignalType>(WorldContext, FuncName);
	}

	static void Bind(UObject* WorldContext, FName FuncName, const UScriptStruct* SignalType)
	{
		const auto SignalBus = GetSignalBus(WorldContext);
		SignalBus->Bind(WorldContext, FuncName, SignalType);
	}

	template <typename T>
	static void Send(UObject* WorldContext, T SignalData)
	{
		const auto SignalBus = GetSignalBus(WorldContext);
		SignalBus->Send<T>(SignalData);
	}

	static void Send(const UObject* WorldContext, const UScriptStruct* SignalType, void* SignalData)
	{
		const auto SignalBus = GetSignalBus(WorldContext);
		SignalBus->Send(SignalType, SignalData);
	}

	template<typename T>
	static void Remove(const UObject* WorldContext, const FName FuncName)
	{
		const auto SignalBus = GetSignalBus(WorldContext);
		SignalBus->Remove<T>(FuncName);
	}

	static void Remove(const UObject* WorldContext, const FName FuncName, const UScriptStruct* SignalType)
	{
		const auto SignalBus = GetSignalBus(WorldContext);
		SignalBus->Remove(FuncName, SignalType);
	}

	static USignalBusService* GetSignalBus(const UObject* WorldContext)
	{
		const auto World = WorldContext->GetWorld();
		checkf(World, TEXT("World not found"))

		const auto GameMode = World->GetAuthGameMode();
		checkf(World, TEXT("Game mode not found"))

		const auto SignalBusComponent = Cast<USignalBusComponent>(
			GameMode->GetComponentByClass(USignalBusComponent::StaticClass()));
		checkf(SignalBusComponent, TEXT("Curent game mode hasn't signal bus interface"))

		return SignalBusComponent->GetSignalBus();
	}
};
