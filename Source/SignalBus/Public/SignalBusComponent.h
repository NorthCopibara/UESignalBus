// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SignalBusService.h"
#include "Components/ActorComponent.h"
#include "SignalBusComponent.generated.h"


UCLASS(ClassGroup=(SignalBus), meta=(BlueprintSpawnableComponent))
class SIGNALBUS_API USignalBusComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USignalBusComponent();

	USignalBusService* GetSignalBus() const;
	
protected:
	virtual void InitializeComponent() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UPROPERTY()
	USignalBusService* SignalBusService = nullptr;
};
