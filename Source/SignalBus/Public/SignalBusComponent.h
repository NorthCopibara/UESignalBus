// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SignalBusComponent.generated.h"

DEFINE_LOG_CATEGORY_STATIC(LogSignalBus, All, All);

#define BindSignal(UserObject, FuncName) Bind(UserObject, FuncName, STATIC_FUNCTION_FNAME(TEXT(#FuncName)))

struct FSignalContext
{
	FSignalContext() = default;
	FSignalContext(const TWeakObjectPtr<UObject>& Context, const TWeakObjectPtr<UFunction>& Func)
		: Context(Context),
		  Func(Func)
	{
	}

	TWeakObjectPtr<UObject> Context = nullptr;
	TWeakObjectPtr<UFunction> Func = nullptr;
};

struct FSignal
{
	TMap<FName, FSignalContext> SignalsContexts {};
};

UCLASS()
class SIGNALBUS_API USignalBusComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	template <class ObjectType, typename SignalType>
	void Bind(UObject* WorldContext, void (ObjectType::*Funk)(SignalType Signal), FName const FuncName);
	template <typename T>
	void Bind(UObject* Context, FName const FuncName);
	void Bind(UObject* Context, FName const FuncName, const UScriptStruct* SignalType);

	template <typename T>
	void Send(T SignalData);
	void Send(const UScriptStruct* SignalStruct, void* SignalData);

	template <typename T>
	void Remove(const FName FuncName);
	void Remove(const FName FuncName, const UScriptStruct* SignalType);
	void Clear();

private:
	TMap<FString, FSignal> Signals;
	
	void RemoveFunction(const FString& SignalName, const FName FuncName);
	void RemoveSignal(const FString& SignalName);
};

template <class ObjectType, typename SignalType>
void USignalBusComponent::Bind(UObject* WorldContext, void(ObjectType::* Funk)(SignalType Signal), FName const FuncName)
{
	Bind<SignalType>(WorldContext, FuncName);
}

template <typename T>
void USignalBusComponent::Bind(UObject* Context, FName const FuncName)
{
	Bind(Context, FuncName, T::StaticStruct());
}

template <typename T>
void USignalBusComponent::Send(T SignalData)
{
	Send(T::StaticStruct(), static_cast<void*>(&SignalData));
}

template <typename T>
void USignalBusComponent::Remove(const FName FuncName)
{
	Remove(FuncName, T::StaticStruct());
}
