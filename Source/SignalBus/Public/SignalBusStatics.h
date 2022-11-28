#pragma once

#include "SignalBusComponent.h"
#include "SignalBusService.h"
#include "GameFramework/GameModeBase.h"


class SignalBusStatics
{
public:
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

	template <typename T>
	static void Bind(UObject* WorldContext, FName FuncName)
	{
		const auto SignalBus = GetSignalBus(WorldContext);
		SignalBus->Bind<T>(WorldContext, FuncName);
	}

	template <typename T>
	static void Send(UObject* WorldContext, T SignalData)
	{
		const auto SignalBus = GetSignalBus(WorldContext);
		SignalBus->Send<T>(SignalData);
	}
};
