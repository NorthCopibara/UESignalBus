#pragma once
#include "SignalBusStatics.h"
#include "Kismet/KismetSystemLibrary.h"

#include "SignalBusBPLibrary.generated.h"

UCLASS()
class SIGNALBUS_API USignalBusBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
/*
	UFUNCTION(
		BlueprintCallable,
		meta = (Keywords = "Bind function to signal bus", WorldContext = "WorldContext"),
		Category = "SignalBus")
	static void BindSignal(UObject* WorldContext, const UScriptStruct* SignalType, FName FunctionName)
	{
		SignalBusStatics::Bind(WorldContext, FunctionName, SignalType);
	}*/

	UFUNCTION(
		BlueprintCallable,
		CustomThunk,
		meta = (Keywords = "Send signal",
			WorldContext = "WorldContext",
			CustomStructureParam = "SignalData",
			AutoCreateRefTerm = "SignalData",
			UnsafeDuringActorConstruction),
		Category = "SignalBus")
	static void SendSignal(const UObject* WorldContext, const UScriptStruct* SignalType, FGenericStruct SignalData)
	{
		checkNoEntry();
	}

	FORCEINLINE static void Generic_SendSignal(const UObject* WorldContext, const UScriptStruct* SignalType,
	                                           void* SignalData)
	{
		//SignalBusStatics::Send(WorldContext, SignalType, SignalData);
	}

	DECLARE_FUNCTION(execSendSignal)
	{
		P_GET_OBJECT(UObject, WorldContextObject);
		P_GET_OBJECT(UScriptStruct, SignalType);

		Stack.StepCompiledIn<FStructProperty>(nullptr);
		void* SrcSignalAddr = Stack.MostRecentPropertyAddress;

		P_FINISH;
		P_NATIVE_BEGIN;
			Generic_SendSignal(WorldContextObject, SignalType, SrcSignalAddr);
		P_NATIVE_END;
	}
};
